#ifndef IMAGE_PATHS_H
#define IMAGE_PATHS_H

#include <QCoreApplication>
#include <QDir>

class ImagePaths {
public:
    static const QString inheritance() {
        QDir cpath = getBasePath();
        return cpath.absoluteFilePath("Uml_inheritance.png");
    }

    static const QString composition() {
        QDir cpath = getBasePath();
        return cpath.absoluteFilePath("Uml_composition.png");
    }

    static const QString aggregation() {
        QDir cpath = getBasePath();
        return cpath.absoluteFilePath("Uml_aggregation.png");
    }

    static const QString association() {
        QDir cpath = getBasePath();
        return cpath.absoluteFilePath("Uml_association.png");
    }

    static const QString dependency() {
        QDir cpath = getBasePath();
        return cpath.absoluteFilePath("Uml_dependency.png");
    }

    static const QString navigation() {
        QDir cpath = getBasePath();
        return cpath.absoluteFilePath("Uml_navigation.png");
    }


private:
    static QDir getBasePath() {
        // QT automatically converts / in \ if OS is windows
        QDir buildPath(QCoreApplication::applicationDirPath());
        buildPath.cd("../../resources/images/");
        return buildPath;
    }
};

#endif // IMAGE_PATHS_H
