/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2017 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef __QTSETUP_H__
#define __QTSETUP_H__

#include <Qt>
#include <QtScript>
#include <QTextDocument>

void setupQt(QScriptEngine *engine);

Q_DECLARE_METATYPE(Qt::Alignment);
Q_DECLARE_METATYPE(enum Qt::AlignmentFlag);
Q_DECLARE_METATYPE(enum Qt::AnchorPoint);
Q_DECLARE_METATYPE(enum Qt::ApplicationAttribute);
Q_DECLARE_METATYPE(enum Qt::ApplicationState);
Q_DECLARE_METATYPE(Qt::ApplicationStates);
Q_DECLARE_METATYPE(enum Qt::ArrowType);
Q_DECLARE_METATYPE(enum Qt::AspectRatioMode);
Q_DECLARE_METATYPE(enum Qt::Axis);
Q_DECLARE_METATYPE(enum Qt::BGMode);
Q_DECLARE_METATYPE(enum Qt::BrushStyle);
Q_DECLARE_METATYPE(enum Qt::CaseSensitivity);
Q_DECLARE_METATYPE(enum Qt::CheckState);
#if QT_VERSION >= 0x050900
Q_DECLARE_METATYPE(enum Qt::ChecksumType);
#endif
Q_DECLARE_METATYPE(enum Qt::ClipOperation);
Q_DECLARE_METATYPE(enum Qt::ConnectionType);
Q_DECLARE_METATYPE(enum Qt::ContextMenuPolicy);
Q_DECLARE_METATYPE(enum Qt::CoordinateSystem);
Q_DECLARE_METATYPE(enum Qt::Corner);
Q_DECLARE_METATYPE(enum Qt::CursorMoveStyle);
Q_DECLARE_METATYPE(enum Qt::CursorShape);
Q_DECLARE_METATYPE(enum Qt::DateFormat);
Q_DECLARE_METATYPE(enum Qt::DayOfWeek);
Q_DECLARE_METATYPE(enum Qt::DockWidgetArea);
Q_DECLARE_METATYPE(Qt::DockWidgetAreas);
Q_DECLARE_METATYPE(enum Qt::DropAction);
Q_DECLARE_METATYPE(Qt::DropActions);
Q_DECLARE_METATYPE(enum Qt::Edge);
Q_DECLARE_METATYPE(Qt::Edges);
#if QT_VERSION >= 0x050900
Q_DECLARE_METATYPE(enum Qt::EnterKeyType);
#endif
Q_DECLARE_METATYPE(enum Qt::EventPriority);
Q_DECLARE_METATYPE(enum Qt::FillRule);
Q_DECLARE_METATYPE(enum Qt::FindChildOption);
Q_DECLARE_METATYPE(Qt::FindChildOptions);
Q_DECLARE_METATYPE(enum Qt::FocusPolicy);
Q_DECLARE_METATYPE(enum Qt::FocusReason);
Q_DECLARE_METATYPE(enum Qt::GestureFlag);
Q_DECLARE_METATYPE(Qt::GestureFlags);
Q_DECLARE_METATYPE(enum Qt::GestureState);
Q_DECLARE_METATYPE(enum Qt::GestureType);
Q_DECLARE_METATYPE(enum Qt::GlobalColor);
Q_DECLARE_METATYPE(enum Qt::HitTestAccuracy);
Q_DECLARE_METATYPE(enum Qt::ImageConversionFlag);
Q_DECLARE_METATYPE(Qt::ImageConversionFlags);
Q_DECLARE_METATYPE(enum Qt::InputMethodHint);
Q_DECLARE_METATYPE(Qt::InputMethodHints);
Q_DECLARE_METATYPE(Qt::InputMethodQueries);
Q_DECLARE_METATYPE(enum Qt::InputMethodQuery);
Q_DECLARE_METATYPE(enum Qt::ItemDataRole);
Q_DECLARE_METATYPE(enum Qt::ItemFlag);
Q_DECLARE_METATYPE(Qt::ItemFlags);
Q_DECLARE_METATYPE(enum Qt::ItemSelectionMode);
Q_DECLARE_METATYPE(enum Qt::ItemSelectionOperation);
Q_DECLARE_METATYPE(enum Qt::Key);
Q_DECLARE_METATYPE(enum Qt::KeyboardModifier);
Q_DECLARE_METATYPE(Qt::KeyboardModifiers);
Q_DECLARE_METATYPE(enum Qt::LayoutDirection);
Q_DECLARE_METATYPE(enum Qt::MaskMode);
Q_DECLARE_METATYPE(enum Qt::MatchFlag);
Q_DECLARE_METATYPE(Qt::MatchFlags);
Q_DECLARE_METATYPE(enum Qt::Modifier);
Q_DECLARE_METATYPE(enum Qt::MouseButton);
Q_DECLARE_METATYPE(Qt::MouseButtons);
Q_DECLARE_METATYPE(enum Qt::MouseEventFlag);
Q_DECLARE_METATYPE(Qt::MouseEventFlags);
Q_DECLARE_METATYPE(enum Qt::MouseEventSource);
Q_DECLARE_METATYPE(enum Qt::NativeGestureType);
Q_DECLARE_METATYPE(enum Qt::NavigationMode);
Q_DECLARE_METATYPE(enum Qt::Orientation);
Q_DECLARE_METATYPE(Qt::Orientations);
Q_DECLARE_METATYPE(enum Qt::PenCapStyle);
Q_DECLARE_METATYPE(enum Qt::PenJoinStyle);
Q_DECLARE_METATYPE(enum Qt::PenStyle);
Q_DECLARE_METATYPE(enum Qt::ScreenOrientation);
Q_DECLARE_METATYPE(Qt::ScreenOrientations);
Q_DECLARE_METATYPE(enum Qt::ScrollBarPolicy);
Q_DECLARE_METATYPE(enum Qt::ScrollPhase);
Q_DECLARE_METATYPE(enum Qt::ShortcutContext);
Q_DECLARE_METATYPE(enum Qt::SizeHint);
Q_DECLARE_METATYPE(enum Qt::SizeMode);
Q_DECLARE_METATYPE(enum Qt::SortOrder);
Q_DECLARE_METATYPE(enum Qt::TabFocusBehavior);
Q_DECLARE_METATYPE(enum Qt::TextElideMode);
Q_DECLARE_METATYPE(enum Qt::TextFlag);
Q_DECLARE_METATYPE(enum Qt::TextFormat);
Q_DECLARE_METATYPE(enum Qt::TextInteractionFlag);
Q_DECLARE_METATYPE(Qt::TextInteractionFlags);
Q_DECLARE_METATYPE(enum Qt::TileRule);
Q_DECLARE_METATYPE(enum Qt::TimeSpec);
Q_DECLARE_METATYPE(enum Qt::TimerType);
Q_DECLARE_METATYPE(enum Qt::ToolBarArea);
Q_DECLARE_METATYPE(Qt::ToolBarAreas);
Q_DECLARE_METATYPE(enum Qt::ToolButtonStyle);
Q_DECLARE_METATYPE(enum Qt::TouchPointState);
Q_DECLARE_METATYPE(Qt::TouchPointStates);
Q_DECLARE_METATYPE(enum Qt::TransformationMode);
Q_DECLARE_METATYPE(enum Qt::UIEffect);
Q_DECLARE_METATYPE(enum Qt::WhiteSpaceMode);
Q_DECLARE_METATYPE(enum Qt::WidgetAttribute);
Q_DECLARE_METATYPE(Qt::WindowFlags);
Q_DECLARE_METATYPE(enum Qt::WindowFrameSection);
Q_DECLARE_METATYPE(enum Qt::WindowModality);
Q_DECLARE_METATYPE(enum Qt::WindowState);
Q_DECLARE_METATYPE(Qt::WindowStates);
Q_DECLARE_METATYPE(enum Qt::WindowType);

#endif
