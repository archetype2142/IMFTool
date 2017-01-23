/* Copyright(C) 2016 Björn Stresing, Denis Manthey, Wolfgang Ruppel, Krispin Weiss
 *
 * This program is free software : you can redistribute it and / or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#include "ImfPackage.h"
#include <QWidget>
#include <QThread>
#include "JP2K_Preview.h"
#include "JP2K_Player.h"
#include "qcombobox.h"
#include "global.h"
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include "qxmlstream.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QTextEdit>
#include <QCheckBox>
#include <math.h> 
#include "createLUTs.h"
#include <QMessageBox>

class WidgetImagePreview;
class QPushButton;
class QLabel;

class WidgetVideoPreview : public QWidget {

	Q_OBJECT

public:
	WidgetVideoPreview(QWidget *pParent = NULL);
	void InstallImp();
	void UninstallImp();
	void setPlaylist(QVector<PlayListElement> &rPlayList, QVector<TTMLtimelineSegment> &rTTMLs);
	JP2K_Player *player;
	~WidgetVideoPreview();
	void Clear();

signals: // ?
	void currentPlayerPosition(qint64);
	void ShowImage(const QImage&);
	void ttmlChanged(const QVector<visibleTTtrack>&, int);
	void regionOptionsChanged(int);
public slots :
	void xPosChanged(const QSharedPointer<AssetMxfTrack>&, const Duration&, const Timecode&, const int&);
	void rShowMsgBox(const QString&, int);
	void rPrevNextSubClicked(bool);
	void getTTML();
private slots:
	void rChangeSpeed(QAction*);
	void rChangeQuality(QAction*);
	void rChangeProcessing(QAction*);
	void rPlayPauseButtonClicked(bool checked);
	void rPlaybackEnded();
	void decodingStatus(qint64, QString);
	void stopPlayback(bool clicked);
protected:
	virtual void keyPressEvent(QKeyEvent *pEvent);

private:
	Q_DISABLE_COPY(WidgetVideoPreview);
	void InitLayout();

	// TTML
	bool showTTML = true;
	QVector<TTMLtimelineSegment> *ttmls;
	QVector<visibleTTtrack> current_tt; // currently visible timed text elements
	QMap<int, QVector<TTMLtimelineSegment>> ttml_tracks;

	QTime ttml_search_time;
	qint64 next_ttml;
	qint64 prev_ttml;

	cpl2016::CompositionPlaylistType mData;

	WidgetImagePreview *mpImagePreview;
	QMessageBox *mpMsgBox;

	// player
	QMenuBar *menuBar;
	QMenu *menuSpeed;
	QAction *speeds[50];
	QMenu *menuQuality;
	QAction *qualities[5];

	QMenu *menuProcessing;
	QMenu *processing_extract;

	QStringList *processing_extract_names;
	QAction *processing_extract_actions[15];
	int processing_extract_action = 3; // default

	QPushButton *mpStopButton;
	QPushButton *mpPlayPauseButton;
	QLabel *labelTest;
	QLabel *decoding_time;
	QLabel *labelImg;
	int current_playlist_index = 0;
	QVector<PlayListElement> currentPlaylist;

	QSharedPointer<AssetMxfTrack> currentAsset;
	QComboBox *set_fps;
	QComboBox *set_layer;

	// player
	int decode_layer = 3; // default
	int decode_speed = 5; // default (fps in player)

	QThread *playerThread;

	JP2K_Preview *decoders[2];
	QThread *decodingThreads[2];
	bool running[2];
	bool now_running = false; // false: 0, true: 1
	int run;
	
	qint64 decodingFrame = 0; // currently decoding frame
	qint64 xSliderFrame = 0;
	qint64 xSliderTotal = 0;
	float xCurrentTime = 0;

	QString xSliderTime; // curent slider position
	QString decodingTime; // current decoding position
};
