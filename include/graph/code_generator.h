#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include <QString>
#include <QVector>
#include <QSet>

class DiagramGraph;
class Composition;

class CodeGenerator {
public:
    explicit CodeGenerator(DiagramGraph* graph);
    bool generate(const QString& outputDir, QString& errorMessage);
    void generate(const QString& outputDir);

private:
    DiagramGraph* m_graph;
    bool topologicalSort(QVector<Composition*>& ordered, QString& errorMessage);
    bool visit(Composition* node, QSet<Composition*>& temporary, QSet<Composition*>& permanent, QVector<Composition*>& ordered);
};

#endif // CODE_GENERATOR_H
