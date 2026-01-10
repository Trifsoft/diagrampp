#include "graph/code_generator.h"
#include "graph/diagram_graph.h"
#include "model/base/uml_class_diagram_node.h"
#include "model/elements/composition/composition.h"

#include <QDir>
#include <QFile>
#include <QTextStream>

CodeGenerator::CodeGenerator(DiagramGraph* graph)
    : m_graph(graph)
{
}

bool CodeGenerator::visit(Composition* node, QSet<Composition*>& temporary, QSet<Composition*>& permanent, QVector<Composition*>& ordered) {
    if (temporary.contains(node)) {
        return false;
    }

    if (permanent.contains(node)) {
        return true;
    }

    temporary.insert(node);

    for (Composition* child : m_graph->links.value(node)) {
        if (!visit(child, temporary, permanent, ordered)) {
            return false;
        }
    }

    temporary.remove(node);
    permanent.insert(node);
    ordered.prepend(node);

    return true;
}

bool CodeGenerator::topologicalSort(QVector<Composition*>& ordered, QString& errorMessage) {
    QSet<Composition*> temporary;
    QSet<Composition*> permanent;

    for (Composition* node : m_graph->links.keys()) {
        if (!permanent.contains(node)) {
            if (!visit(node, temporary, permanent, ordered)) {
                errorMessage = "Inheritance cycle detected in UML diagram.";
                return false;
            }
        }
    }

    return true;
}



bool CodeGenerator::generate(const QString& outputDir, QString& errorMessage) {
    QVector<Composition*> ordered;

    if (!topologicalSort(ordered, errorMessage)) {
        return false;
    }

    QDir dir(outputDir);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QSet<Composition*> generated;

    for (Composition* comp : ordered) {
        QString baseName = comp->get_name();

        QFile hFile(outputDir + "/" + baseName + ".h");
        if (hFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&hFile);
            out << comp->declaration();
        }

        QFile cppFile(outputDir + "/" + baseName + ".cpp");
        if (cppFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&cppFile);
            out << comp->definition();
        }

        generated.insert(comp);
    }

    for (IUMLClassDiagramNode* node : m_graph->nodes) {
        Composition* comp = dynamic_cast<Composition*>(node);
        if (!comp || generated.contains(comp)) {
            continue;
        }

        QString baseName = comp->get_name();

        QFile hFile(outputDir + "/" + baseName + ".h");
        if (hFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&hFile);
            out << comp->declaration();
        }

        QFile cppFile(outputDir + "/" + baseName + ".cpp");
        if (cppFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&cppFile);
            out << comp->definition();
        }
    }

    return true;
}
