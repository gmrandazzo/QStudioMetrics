/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Implementation of PluginManager.
 *
 * Copyright (C) 2016-2026 Giuseppe Marco Randazzo <gmrandazzo@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 */

#include "PluginManager.h"
#include <QDir>
#include <QCoreApplication>
#include <QDebug>
#include "qsmdata.h"

PluginManager& PluginManager::instance() {
    static PluginManager inst;
    return inst;
}

void PluginManager::loadPlugins(PROJECTS* projects, QWidget* parent) {
    QDir pluginsDir(QCoreApplication::applicationDirPath());

    // Search for the plugins directory
#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#endif

    if (!pluginsDir.cd("plugins")) {
#ifdef DEBUG
        qDebug() << "Plugin directory 'plugins' not found relative to app path.";
#endif
        return;
    }

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *pluginObject = loader.instance();
        if (pluginObject) {
            auto *basePlugin = qobject_cast<QSMPluginInterface*>(pluginObject);
            if (basePlugin) {
                basePlugin->initialize(projects, parent);
                m_genericPlugins.append(basePlugin);

                // Check for report generator interface
                auto *reportGen = qobject_cast<IReportGenerator*>(pluginObject);
                if (reportGen) {
                    m_reportGenerators.append(reportGen);
                }

                // Check for context menu interface
                auto *contextMenuPlugin = qobject_cast<IContextMenuPlugin*>(pluginObject);
                if (contextMenuPlugin) {
                    m_contextMenuPlugins.append(contextMenuPlugin);
                }
#ifdef DEBUG
                qDebug() << "Successfully loaded plugin:" << basePlugin->pluginName();
#endif
            }
        } else {
#ifdef DEBUG
            qDebug() << "Failed to load plugin:" << fileName << loader.errorString();
#endif
        }
    }
}
