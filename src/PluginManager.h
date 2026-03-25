/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Plugin manager to discover and load QStudioMetrics plugins.
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

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <QList>
#include <QPluginLoader>
#include <qsm-sdk/QSMInterfaces.h>

class PluginManager : public QObject {
    Q_OBJECT
public:
    static PluginManager& instance();

    /**
     * @brief Load all plugins from the executable's plugins directory.
     * @param projects Access to the projects data.
     * @param parent The MainWindow widget for context.
     */
    void loadPlugins(PROJECTS* projects, QWidget* parent);

    /** @return List of loaded report generators */
    QList<IReportGenerator*> reportGenerators() const { return m_reportGenerators; }

    /** @return List of loaded context menu plugins */
    QList<IContextMenuPlugin*> contextMenuPlugins() const { return m_contextMenuPlugins; }

    /** @return List of all generic plugins */
    QList<QSMPluginInterface*> genericPlugins() const { return m_genericPlugins; }

private:
    explicit PluginManager(QObject *parent = nullptr) : QObject(parent) {}
    QList<IReportGenerator*> m_reportGenerators;
    QList<IContextMenuPlugin*> m_contextMenuPlugins;
    QList<QSMPluginInterface*> m_genericPlugins;
};

#endif // PLUGINMANAGER_H
