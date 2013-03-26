/***************************************************************************
 *   Copyright (C) 2013 by Andrey Afletdinov <fheroes2@gmail.com>          *
 *                                                                         *
 *   Part of the Free Heroes2 Engine:                                      *
 *   http://sourceforge.net/projects/fheroes2                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QtGui>
#include <QContextMenuEvent>

#include "mainwindow.h"
#include "mapwindow.h"

ActionGround::ActionGround(int ground, QObject* parent) : QAction(parent), type(ground)
{
    switch(ground)
    {
	case Ground::Desert:
	    setIcon(QIcon(":/images/ground_desert.png"));
	    setText(tr("Desert"));
	    setStatusTip(tr("Select desert ground"));
	    break;
	case Ground::Snow:
	    setIcon(QIcon(":/images/ground_snow.png"));
	    setText(tr("Snow"));
	    setStatusTip(tr("Select snow ground"));
	    break;
	case Ground::Swamp:
	    setIcon(QIcon(":/images/ground_swamp.png"));
	    setText(tr("Swamp"));
	    setStatusTip(tr("Select swamp ground"));
	    break;
	case Ground::Wasteland:
	    setIcon(QIcon(":/images/ground_wasteland.png"));
	    setText(tr("Wasteland"));
	    setStatusTip(tr("Select wasteland ground"));
	    break;
	case Ground::Beach:
	    setIcon(QIcon(":/images/ground_beach.png"));
	    setText(tr("Beach"));
	    setStatusTip(tr("Select beach ground"));
	    break;
	case Ground::Lava:
	    setIcon(QIcon(":/images/ground_lava.png"));
	    setText(tr("Lava"));
	    setStatusTip(tr("Select lava ground"));
	    break;
	case Ground::Dirt:
	    setIcon(QIcon(":/images/ground_dirt.png"));
	    setText(tr("Dirt"));
	    setStatusTip(tr("Select dirt ground"));
	    break;
	case Ground::Grass:
	    setIcon(QIcon(":/images/ground_grass.png"));
	    setText(tr("Grass"));
	    setStatusTip(tr("Select grass ground"));
	    break;
	case Ground::Water:
	    setIcon(QIcon(":/images/ground_water.png"));
	    setText(tr("Water"));
	    setStatusTip(tr("Select water"));
	    break;
    }
}

MapWindow::MapWindow(MainWindow* parent) : mainWindow(parent), mapData(this)
{
    setAttribute(Qt::WA_DeleteOnClose);
    isUntitled = true;
    isModified = false;

    // init: fill ground
    fillGroundAct = new QActionGroup(this);

    fillGroundAct->addAction(new ActionGround(Ground::Desert, fillGroundAct));
    fillGroundAct->addAction(new ActionGround(Ground::Snow, fillGroundAct));
    fillGroundAct->addAction(new ActionGround(Ground::Swamp, fillGroundAct));
    fillGroundAct->addAction(new ActionGround(Ground::Wasteland, fillGroundAct));
    fillGroundAct->addAction(new ActionGround(Ground::Beach, fillGroundAct));
    fillGroundAct->addAction(new ActionGround(Ground::Lava, fillGroundAct));
    fillGroundAct->addAction(new ActionGround(Ground::Dirt, fillGroundAct));
    fillGroundAct->addAction(new ActionGround(Ground::Grass, fillGroundAct));
    fillGroundAct->addAction(new ActionGround(Ground::Water, fillGroundAct));
    connect(fillGroundAct, SIGNAL(triggered(QAction*)), this, SLOT(fillGroundAction(QAction*)));

    // clear objects acvtion
    clearObjectsAct = new QAction(QIcon(":/images/clear_objects.png"), tr("&Remove Objects"), this);
    clearObjectsAct->setStatusTip(tr("Clear objects"));
    connect(clearObjectsAct, SIGNAL(triggered()), this, SLOT(clearObjectsAction()));
}

void MapWindow::newFile(const QSize & sz, int sequenceNumber)
{
    isUntitled = true;

    QString fileName;
    QTextStream ss(& fileName);

    ss << "map_";
    ss.setFieldWidth(4);
    ss.setPadChar('0');
    ss << sequenceNumber;
    ss.setFieldWidth(0);
    ss << ".xml";

    QApplication::setOverrideCursor(Qt::WaitCursor);

    mapData.newMap(sz, curFile);
    setScene(& mapData);

    QApplication::restoreOverrideCursor();
    curFile = fileName;

    connect(&mapData, SIGNAL(selectionChanged(void)),
		    this, SLOT(mapWasSelectionChanged(void)));

    connect(&mapData, SIGNAL(dataModified(void)),
        	    this, SLOT(mapWasModified(void)));

    mapWasModified();
}

bool MapWindow::loadFile(const QString & fileName)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    if(! mapData.loadMap(fileName))
    {
	QApplication::restoreOverrideCursor();
        QMessageBox::warning(this, tr("Map Editor"),
                             tr("Cannot read file %1.")
                             .arg(fileName));
        return false;
    }

    setScene(& mapData);

    QApplication::restoreOverrideCursor();
    setCurrentFile(fileName);

    connect(&mapData, SIGNAL(selectionChanged(void)),
		    this, SLOT(mapWasSelectionChanged(void)));

    connect(&mapData, SIGNAL(dataModified(void)),
        	    this, SLOT(mapWasModified(void)));

    return true;
}

bool MapWindow::save(void)
{
    return isUntitled ? saveAs() : saveFile(curFile);
}

bool MapWindow::saveAs(void)
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                    curFile);
    return fileName.isEmpty() ? false : saveFile(fileName);
}

bool MapWindow::saveFile(const QString & fileName)
{
    Q_UNUSED(fileName);
/*
    QFile file(fileName);

    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Map Editor"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << toPlainText();

    QApplication::restoreOverrideCursor();
    setCurrentFile(fileName);

*/
    return true;
}

QString MapWindow::userFriendlyCurrentFile(void)
{
    return mapData.name() + " (" + strippedName(curFile) + ")";
}

QString MapWindow::currentFile(void)
{
    return curFile;
}

void MapWindow::closeEvent(QCloseEvent* event)
{
    if(maybeSave())
        event->accept();
    else
        event->ignore();
}

void MapWindow::mapWasSelectionChanged(void)
{
    emit copyAvailable(mapData.selectedItems().size());
}

void MapWindow::mapWasModified(void)
{
    isModified = true;
    setWindowTitle(userFriendlyCurrentFile() + "[*]");
    setWindowModified(true);
}

bool MapWindow::maybeSave(void)
{
/*
    if(isModified)
    {
	QMessageBox::StandardButton ret;

        ret = QMessageBox::warning(this, tr("Map Editor"),
                     tr("'%1' has been modified.\n"
		     "Do you want to save your changes?") .arg(userFriendlyCurrentFile()),
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if(ret == QMessageBox::Save)
            return save();
        else
	if(ret == QMessageBox::Cancel)
    	    return false;
    }
*/
    return true;
}

void MapWindow::setCurrentFile(const QString & fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    isUntitled = false;
    isModified = false;
    setWindowModified(false);
    setWindowTitle(userFriendlyCurrentFile() + "[*]");
}

QString MapWindow::strippedName(const QString & fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

int MapWindow::modeView(void) const
{
    return mapData.sceneModeView();
}

void MapWindow::setModeView(int mode)
{
    mapData.clearSelection();

    switch(mode)
    {
	// explore
	case 1:
	    break;

	// select
	case 2:
	    break;

	default: break;
    }

    mapData.setSceneModeView(mode);
    mapData.update();
}

void MapWindow::contextMenuEvent(QContextMenuEvent* event)
{
    if(mapData.selectedItems().size())
    {
	QMenu menu(this);

	menu.addAction(mainWindow->editCopyAct);
	menu.addAction(mainWindow->editPasteAct);
	menu.addSeparator();

	QMenu* ground = menu.addMenu(QIcon(":/images/menu_fill.png"), tr("&Fill Ground"));
	QList<QAction*> actions = fillGroundAct->actions();

	for(QList<QAction*>::const_iterator
	    it = actions.begin(); it != actions.end(); ++it)
	    ground->addAction(*it);

	menu.addSeparator();
	menu.addAction(clearObjectsAct);

	menu.exec(event->globalPos());
	mapData.clearSelection();
    }
}

void MapWindow::fillGroundAction(QAction* act)
{
    ActionGround* groundAct = qobject_cast<ActionGround*>(act);

    if(groundAct)
    {
	isModified = true;
	qDebug() << "fill ground: " << groundAct->ground();
    }
}

void MapWindow::clearObjectsAction(void)
{
    QList<QGraphicsItem*> selected = mapData.selectedItems();

    if(selected.size())
    {
    }
}

void MapWindow::copy(void)
{
    //QList<QGraphicsItem*> = mapData.selectedItems();
}

void MapWindow::paste(void)
{
    isModified = true;
}
