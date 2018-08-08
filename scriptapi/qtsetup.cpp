/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2017 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "qtsetup.h"

// To Script Values ////////////////////////////////////////////////////////////

QScriptValue AlignmentToScriptValue(QScriptEngine *engine, const Qt::Alignment &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue AlignmentFlagToScriptValue(QScriptEngine *engine, const enum Qt::AlignmentFlag &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue AnchorPointToScriptValue(QScriptEngine *engine, const enum Qt::AnchorPoint &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue ApplicationAttributeToScriptValue(QScriptEngine *engine, const enum Qt::ApplicationAttribute &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue ApplicationStateToScriptValue(QScriptEngine *engine, const enum Qt::ApplicationState &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue ApplicationStatesToScriptValue(QScriptEngine *engine, const Qt::ApplicationStates &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue ArrowTypeToScriptValue(QScriptEngine *engine, const enum Qt::ArrowType &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue AspectRatioModeToScriptValue(QScriptEngine *engine, const enum Qt::AspectRatioMode &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue AxisToScriptValue(QScriptEngine *engine, const enum Qt::Axis &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue BGModeToScriptValue(QScriptEngine *engine, const enum Qt::BGMode &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue BrushStyleToScriptValue(QScriptEngine *engine, const enum Qt::BrushStyle &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue CaseSensitivityToScriptValue(QScriptEngine *engine, const enum Qt::CaseSensitivity &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue CheckStateToScriptValue(QScriptEngine *engine, const enum Qt::CheckState &p)
{
  return QScriptValue(engine, (int)p);
}

#if QT_VERSION >= 0x050900
QScriptValue ChecksumTypeToScriptValue(QScriptEngine *engine, const enum Qt::ChecksumType &p)
{
  return QScriptValue(engine, (int)p);
}
#endif

QScriptValue ClipOperationToScriptValue(QScriptEngine *engine, const enum Qt::ClipOperation &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue ConnectionTypeToScriptValue(QScriptEngine *engine, const enum Qt::ConnectionType &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue ContextMenuPolicyToScriptValue(QScriptEngine *engine, const enum Qt::ContextMenuPolicy &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue CoordinateSystemToScriptValue(QScriptEngine *engine, const enum Qt::CoordinateSystem &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue CornerToScriptValue(QScriptEngine *engine, const enum Qt::Corner &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue CursorMoveStyleToScriptValue(QScriptEngine *engine, const enum Qt::CursorMoveStyle &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue CursorShapeToScriptValue(QScriptEngine *engine, const enum Qt::CursorShape &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue DateFormatToScriptValue(QScriptEngine *engine, const enum Qt::DateFormat &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue DayOfWeekToScriptValue(QScriptEngine *engine, const enum Qt::DayOfWeek &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue DockWidgetAreaToScriptValue(QScriptEngine *engine, const enum Qt::DockWidgetArea &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue DockWidgetAreasToScriptValue(QScriptEngine *engine, const Qt::DockWidgetAreas &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue DropActionToScriptValue(QScriptEngine *engine, const enum Qt::DropAction &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue DropActionsToScriptValue(QScriptEngine *engine, const Qt::DropActions &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue EdgeToScriptValue(QScriptEngine *engine, const enum Qt::Edge &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue EdgesToScriptValue(QScriptEngine *engine, const Qt::Edges &p)
{
  return QScriptValue(engine, (int)p);
}

#if QT_VERSION >= 0x050600
QScriptValue EnterKeyTypeToScriptValue(QScriptEngine *engine, const enum Qt::EnterKeyType &p)
{
  return QScriptValue(engine, (int)p);
}
#endif

QScriptValue EventPriorityToScriptValue(QScriptEngine *engine, const enum Qt::EventPriority &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue FillRuleToScriptValue(QScriptEngine *engine, const enum Qt::FillRule &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue FindChildOptionToScriptValue(QScriptEngine *engine, const enum Qt::FindChildOption &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue FindChildOptionsToScriptValue(QScriptEngine *engine, const Qt::FindChildOptions &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue FocusPolicyToScriptValue(QScriptEngine *engine, const Qt::FocusPolicy &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue FocusReasonToScriptValue(QScriptEngine *engine, const enum Qt::FocusReason &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue GestureFlagToScriptValue(QScriptEngine *engine, const enum Qt::GestureFlag &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue GestureFlagsToScriptValue(QScriptEngine *engine, const Qt::GestureFlags &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue GestureStateToScriptValue(QScriptEngine *engine, const enum Qt::GestureState &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue GestureTypeToScriptValue(QScriptEngine *engine, const enum Qt::GestureType &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue GlobalColorToScriptValue(QScriptEngine *engine, const enum Qt::GlobalColor &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue HitTestAccuracyToScriptValue(QScriptEngine *engine, const enum Qt::HitTestAccuracy &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue ImageConversionFlagToScriptValue(QScriptEngine *engine, const enum Qt::ImageConversionFlag &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue ImageConversionFlagsToScriptValue(QScriptEngine *engine, const Qt::ImageConversionFlags &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue InputMethodHintToScriptValue(QScriptEngine *engine, const enum Qt::InputMethodHint &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue InputMethodHintsToScriptValue(QScriptEngine *engine, const Qt::InputMethodHints &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue InputMethodQueriesToScriptValue(QScriptEngine *engine, const Qt::InputMethodQueries &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue InputMethodQueryToScriptValue(QScriptEngine *engine, const enum Qt::InputMethodQuery &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue ItemDataRoleToScriptValue(QScriptEngine *engine, const enum Qt::ItemDataRole &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue ItemFlagToScriptValue(QScriptEngine *engine, const enum Qt::ItemFlag &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue ItemFlagsToScriptValue(QScriptEngine *engine, const Qt::ItemFlags &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue ItemSelectionModeToScriptValue(QScriptEngine *engine, const enum Qt::ItemSelectionMode &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue ItemSelectionOperationToScriptValue(QScriptEngine *engine, const enum Qt::ItemSelectionOperation &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue KeyToScriptValue(QScriptEngine *engine, const enum Qt::Key &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue KeyboardModifierToScriptValue(QScriptEngine *engine, const enum Qt::KeyboardModifier &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue KeyboardModifiersToScriptValue(QScriptEngine *engine, const Qt::KeyboardModifiers &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue LayoutDirectionToScriptValue(QScriptEngine *engine, const enum Qt::LayoutDirection &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue MaskModeToScriptValue(QScriptEngine *engine, const enum Qt::MaskMode &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue MatchFlagToScriptValue(QScriptEngine *engine, const enum Qt::MatchFlag &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue MatchFlagsToScriptValue(QScriptEngine *engine, const Qt::MatchFlags &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue ModifierToScriptValue(QScriptEngine *engine, const enum Qt::Modifier &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue MouseButtonToScriptValue(QScriptEngine *engine, const enum Qt::MouseButton &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue MouseButtonsToScriptValue(QScriptEngine *engine, const Qt::MouseButtons &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue MouseEventFlagToScriptValue(QScriptEngine *engine, const enum Qt::MouseEventFlag &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue MouseEventFlagsToScriptValue(QScriptEngine *engine, const Qt::MouseEventFlags &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue MouseEventSourceToScriptValue(QScriptEngine *engine, const enum Qt::MouseEventSource &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue NativeGestureTypeToScriptValue(QScriptEngine *engine, const enum Qt::NativeGestureType &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue NavigationModeToScriptValue(QScriptEngine *engine, const enum Qt::NavigationMode &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue OrientationToScriptValue(QScriptEngine *engine, const enum Qt::Orientation &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue OrientationsToScriptValue(QScriptEngine *engine, const Qt::Orientations &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue PenCapStyleToScriptValue(QScriptEngine *engine, const enum Qt::PenCapStyle &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue PenJoinStyleToScriptValue(QScriptEngine *engine, const enum Qt::PenJoinStyle &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue PenStyleToScriptValue(QScriptEngine *engine, const enum Qt::PenStyle &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue ScreenOrientationToScriptValue(QScriptEngine *engine, const enum Qt::ScreenOrientation &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue ScreenOrientationsToScriptValue(QScriptEngine *engine, const Qt::ScreenOrientations &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue ScrollBarPolicyToScriptValue(QScriptEngine *engine, const enum Qt::ScrollBarPolicy &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue ScrollPhaseToScriptValue(QScriptEngine *engine, const enum Qt::ScrollPhase &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue ShortcutContextToScriptValue(QScriptEngine *engine, const enum Qt::ShortcutContext &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue SizeHintToScriptValue(QScriptEngine *engine, const enum Qt::SizeHint &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue SizeModeToScriptValue(QScriptEngine *engine, const enum Qt::SizeMode &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue SortOrderToScriptValue(QScriptEngine *engine, const enum Qt::SortOrder &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue TabFocusBehaviorToScriptValue(QScriptEngine *engine, const enum Qt::TabFocusBehavior &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue TextElideModeToScriptValue(QScriptEngine *engine, const enum Qt::TextElideMode &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue TextFlagToScriptValue(QScriptEngine *engine, const enum Qt::TextFlag &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue TextFormatToScriptValue(QScriptEngine *engine, const enum Qt::TextFormat &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue TextInteractionFlagToScriptValue(QScriptEngine *engine, const enum Qt::TextInteractionFlag &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue TextInteractionFlagsToScriptValue(QScriptEngine *engine, const Qt::TextInteractionFlags &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue TileRuleToScriptValue(QScriptEngine *engine, const enum Qt::TileRule &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue TimeSpecToScriptValue(QScriptEngine *engine, const enum Qt::TimeSpec &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue TimerTypeToScriptValue(QScriptEngine *engine, const enum Qt::TimerType &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue ToolBarAreaToScriptValue(QScriptEngine *engine, const enum Qt::ToolBarArea &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue ToolBarAreasToScriptValue(QScriptEngine *engine, const Qt::ToolBarAreas &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue ToolButtonStyleToScriptValue(QScriptEngine *engine, const enum Qt::ToolButtonStyle &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue TouchPointStateToScriptValue(QScriptEngine *engine, const enum Qt::TouchPointState &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue TouchPointStatesToScriptValue(QScriptEngine *engine, const Qt::TouchPointStates &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue TransformationModeToScriptValue(QScriptEngine *engine, const enum Qt::TransformationMode &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue UIEffectToScriptValue(QScriptEngine *engine, const enum Qt::UIEffect &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue WhiteSpaceModeToScriptValue(QScriptEngine *engine, const enum Qt::WhiteSpaceMode &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue WidgetAttributeToScriptValue(QScriptEngine *engine, const enum Qt::WidgetAttribute &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue WindowFlagsToScriptValue(QScriptEngine *engine, const Qt::WindowFlags &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue WindowFrameSectionToScriptValue(QScriptEngine *engine, const enum Qt::WindowFrameSection &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue WindowModalityToScriptValue(QScriptEngine *engine, const enum Qt::WindowModality &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue WindowStateToScriptValue(QScriptEngine *engine, const enum Qt::WindowState &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue WindowStatesToScriptValue(QScriptEngine *engine, const Qt::WindowStates &p)
{
  return QScriptValue(engine, (int)p);
}

QScriptValue WindowTypeToScriptValue(QScriptEngine *engine, const enum Qt::WindowType &p)
{
  return QScriptValue(engine, (int)p);
}

// From Script Values //////////////////////////////////////////////////////////

void AlignmentFromScriptValue(const QScriptValue &obj, Qt::Alignment &p)
{
  p = (Qt::Alignment)obj.toInt32();
}

void AlignmentFlagFromScriptValue(const QScriptValue &obj, enum Qt::AlignmentFlag &p)
{
  p = (enum Qt::AlignmentFlag)obj.toInt32();
}

void AnchorPointFromScriptValue(const QScriptValue &obj, enum Qt::AnchorPoint &p)
{
  p = (enum Qt::AnchorPoint)obj.toInt32();
}

void ApplicationAttributeFromScriptValue(const QScriptValue &obj, enum Qt::ApplicationAttribute &p)
{
  p = (enum Qt::ApplicationAttribute)obj.toInt32();
}

void ApplicationStateFromScriptValue(const QScriptValue &obj, enum Qt::ApplicationState &p)
{
  p = (enum Qt::ApplicationState)obj.toInt32();
}

void ApplicationStatesFromScriptValue(const QScriptValue &obj, Qt::ApplicationStates &p)
{
  p = (Qt::ApplicationStates)obj.toInt32();
}

void ArrowTypeFromScriptValue(const QScriptValue &obj, enum Qt::ArrowType &p)
{
  p = (enum Qt::ArrowType)obj.toInt32();
}

void AspectRatioModeFromScriptValue(const QScriptValue &obj, enum Qt::AspectRatioMode &p)
{
  p = (enum Qt::AspectRatioMode)obj.toInt32();
}

void AxisFromScriptValue(const QScriptValue &obj, enum Qt::Axis &p)
{
  p = (enum Qt::Axis)obj.toInt32();
}

void BGModeFromScriptValue(const QScriptValue &obj, enum Qt::BGMode &p)
{
  p = (enum Qt::BGMode)obj.toInt32();
}

void BrushStyleFromScriptValue(const QScriptValue &obj, enum Qt::BrushStyle &p)
{
  p = (enum Qt::BrushStyle)obj.toInt32();
}

void CaseSensitivityFromScriptValue(const QScriptValue &obj, enum Qt::CaseSensitivity &p)
{
  p = (enum Qt::CaseSensitivity)obj.toInt32();
}

void CheckStateFromScriptValue(const QScriptValue &obj, enum Qt::CheckState &p)
{
  p = (enum Qt::CheckState)obj.toInt32();
}

#if QT_VERSION >= 0x050900
void ChecksumTypeFromScriptValue(const QScriptValue &obj, enum Qt::ChecksumType &p)
{
  p = (enum Qt::ChecksumType)obj.toInt32();
}
#endif

void ClipOperationFromScriptValue(const QScriptValue &obj, enum Qt::ClipOperation &p)
{
  p = (enum Qt::ClipOperation)obj.toInt32();
}

void ConnectionTypeFromScriptValue(const QScriptValue &obj, enum Qt::ConnectionType &p)
{
  p = (enum Qt::ConnectionType)obj.toInt32();
}

void ContextMenuPolicyFromScriptValue(const QScriptValue &obj, enum Qt::ContextMenuPolicy &p)
{
  p = (enum Qt::ContextMenuPolicy)obj.toInt32();
}

void CoordinateSystemFromScriptValue(const QScriptValue &obj, enum Qt::CoordinateSystem &p)
{
  p = (enum Qt::CoordinateSystem)obj.toInt32();
}

void CornerFromScriptValue(const QScriptValue &obj, enum Qt::Corner &p)
{
  p = (enum Qt::Corner)obj.toInt32();
}

void CursorMoveStyleFromScriptValue(const QScriptValue &obj, enum Qt::CursorMoveStyle &p)
{
  p = (enum Qt::CursorMoveStyle)obj.toInt32();
}

void CursorShapeFromScriptValue(const QScriptValue &obj, enum Qt::CursorShape &p)
{
  p = (enum Qt::CursorShape)obj.toInt32();
}

void DateFormatFromScriptValue(const QScriptValue &obj, enum Qt::DateFormat &p)
{
  p = (enum Qt::DateFormat)obj.toInt32();
}

void DayOfWeekFromScriptValue(const QScriptValue &obj, enum Qt::DayOfWeek &p)
{
  p = (enum Qt::DayOfWeek)obj.toInt32();
}

void DockWidgetAreaFromScriptValue(const QScriptValue &obj, enum Qt::DockWidgetArea &p)
{
  p = (enum Qt::DockWidgetArea)obj.toInt32();
}

void DockWidgetAreasFromScriptValue(const QScriptValue &obj, Qt::DockWidgetAreas &p)
{
  p = (Qt::DockWidgetAreas)obj.toInt32();
}

void DropActionFromScriptValue(const QScriptValue &obj, enum Qt::DropAction &p)
{
  p = (enum Qt::DropAction)obj.toInt32();
}

void DropActionsFromScriptValue(const QScriptValue &obj, Qt::DropActions &p)
{
  p = (Qt::DropActions)obj.toInt32();
}

void EdgeFromScriptValue(const QScriptValue &obj, enum Qt::Edge &p)
{
  p = (enum Qt::Edge)obj.toInt32();
}

void EdgesFromScriptValue(const QScriptValue &obj, Qt::Edges &p)
{
  p = (Qt::Edges)obj.toInt32();
}

#if QT_VERSION >= 0x050600
void EnterKeyTypeFromScriptValue(const QScriptValue &obj, enum Qt::EnterKeyType &p)
{
  p = (enum Qt::EnterKeyType)obj.toInt32();
}
#endif

void EventPriorityFromScriptValue(const QScriptValue &obj, enum Qt::EventPriority &p)
{
  p = (enum Qt::EventPriority)obj.toInt32();
}

void FillRuleFromScriptValue(const QScriptValue &obj, enum Qt::FillRule &p)
{
  p = (enum Qt::FillRule)obj.toInt32();
}

void FindChildOptionFromScriptValue(const QScriptValue &obj, enum Qt::FindChildOption &p)
{
  p = (enum Qt::FindChildOption)obj.toInt32();
}

void FindChildOptionsFromScriptValue(const QScriptValue &obj, Qt::FindChildOptions &p)
{
  p = (Qt::FindChildOptions)obj.toInt32();
}

void FocusPolicyFromScriptValue(const QScriptValue &obj, Qt::FocusPolicy &p)
{
  p = (Qt::FocusPolicy)obj.toInt32();
}

void FocusReasonFromScriptValue(const QScriptValue &obj, enum Qt::FocusReason &p)
{
  p = (enum Qt::FocusReason)obj.toInt32();
}

void GestureFlagFromScriptValue(const QScriptValue &obj, enum Qt::GestureFlag &p)
{
  p = (enum Qt::GestureFlag)obj.toInt32();
}

void GestureFlagsFromScriptValue(const QScriptValue &obj, Qt::GestureFlags &p)
{
  p = (Qt::GestureFlags)obj.toInt32();
}

void GestureStateFromScriptValue(const QScriptValue &obj, enum Qt::GestureState &p)
{
  p = (enum Qt::GestureState)obj.toInt32();
}

void GestureTypeFromScriptValue(const QScriptValue &obj, enum Qt::GestureType &p)
{
  p = (enum Qt::GestureType)obj.toInt32();
}

void GlobalColorFromScriptValue(const QScriptValue &obj, enum Qt::GlobalColor &p)
{
  p = (enum Qt::GlobalColor)obj.toInt32();
}

void HitTestAccuracyFromScriptValue(const QScriptValue &obj, enum Qt::HitTestAccuracy &p)
{
  p = (enum Qt::HitTestAccuracy)obj.toInt32();
}

void ImageConversionFlagFromScriptValue(const QScriptValue &obj, enum Qt::ImageConversionFlag &p)
{
  p = (enum Qt::ImageConversionFlag)obj.toInt32();
}

void ImageConversionFlagsFromScriptValue(const QScriptValue &obj, Qt::ImageConversionFlags &p)
{
  p = (Qt::ImageConversionFlags)obj.toInt32();
}

void InputMethodHintFromScriptValue(const QScriptValue &obj, enum Qt::InputMethodHint &p)
{
  p = (enum Qt::InputMethodHint)obj.toInt32();
}

void InputMethodHintsFromScriptValue(const QScriptValue &obj, Qt::InputMethodHints &p)
{
  p = (Qt::InputMethodHints)obj.toInt32();
}

void InputMethodQueriesFromScriptValue(const QScriptValue &obj, Qt::InputMethodQueries &p)
{
  p = (Qt::InputMethodQueries)obj.toInt32();
}

void InputMethodQueryFromScriptValue(const QScriptValue &obj, enum Qt::InputMethodQuery &p)
{
  p = (enum Qt::InputMethodQuery)obj.toInt32();
}

void ItemDataRoleFromScriptValue(const QScriptValue &obj, enum Qt::ItemDataRole &p)
{
  p = (enum Qt::ItemDataRole)obj.toInt32();
}

void ItemFlagFromScriptValue(const QScriptValue &obj, enum Qt::ItemFlag &p)
{
  p = (enum Qt::ItemFlag)obj.toInt32();
}

void ItemFlagsFromScriptValue(const QScriptValue &obj, Qt::ItemFlags &p)
{
  p = (Qt::ItemFlags)obj.toInt32();
}

void ItemSelectionModeFromScriptValue(const QScriptValue &obj, enum Qt::ItemSelectionMode &p)
{
  p = (enum Qt::ItemSelectionMode)obj.toInt32();
}

void ItemSelectionOperationFromScriptValue(const QScriptValue &obj, enum Qt::ItemSelectionOperation &p)
{
  p = (enum Qt::ItemSelectionOperation)obj.toInt32();
}

void KeyFromScriptValue(const QScriptValue &obj, enum Qt::Key &p)
{
  p = (enum Qt::Key)obj.toInt32();
}

void KeyboardModifierFromScriptValue(const QScriptValue &obj, enum Qt::KeyboardModifier &p)
{
  p = (enum Qt::KeyboardModifier)obj.toInt32();
}

void KeyboardModifiersFromScriptValue(const QScriptValue &obj, Qt::KeyboardModifiers &p)
{
  p = (Qt::KeyboardModifiers)obj.toInt32();
}

void LayoutDirectionFromScriptValue(const QScriptValue &obj, enum Qt::LayoutDirection &p)
{
  p = (enum Qt::LayoutDirection)obj.toInt32();
}

void MaskModeFromScriptValue(const QScriptValue &obj, enum Qt::MaskMode &p)
{
  p = (enum Qt::MaskMode)obj.toInt32();
}

void MatchFlagFromScriptValue(const QScriptValue &obj, enum Qt::MatchFlag &p)
{
  p = (enum Qt::MatchFlag)obj.toInt32();
}

void MatchFlagsFromScriptValue(const QScriptValue &obj, Qt::MatchFlags &p)
{
  p = (Qt::MatchFlags)obj.toInt32();
}

void ModifierFromScriptValue(const QScriptValue &obj, enum Qt::Modifier &p)
{
  p = (enum Qt::Modifier)obj.toInt32();
}

void MouseButtonFromScriptValue(const QScriptValue &obj, enum Qt::MouseButton &p)
{
  p = (enum Qt::MouseButton)obj.toInt32();
}

void MouseButtonsFromScriptValue(const QScriptValue &obj, Qt::MouseButtons &p)
{
  p = (Qt::MouseButtons)obj.toInt32();
}

void MouseEventFlagFromScriptValue(const QScriptValue &obj, enum Qt::MouseEventFlag &p)
{
  p = (enum Qt::MouseEventFlag)obj.toInt32();
}

void MouseEventFlagsFromScriptValue(const QScriptValue &obj, Qt::MouseEventFlags &p)
{
  p = (Qt::MouseEventFlags)obj.toInt32();
}

void MouseEventSourceFromScriptValue(const QScriptValue &obj, enum Qt::MouseEventSource &p)
{
  p = (enum Qt::MouseEventSource)obj.toInt32();
}

void NativeGestureTypeFromScriptValue(const QScriptValue &obj, enum Qt::NativeGestureType &p)
{
  p = (enum Qt::NativeGestureType)obj.toInt32();
}

void NavigationModeFromScriptValue(const QScriptValue &obj, enum Qt::NavigationMode &p)
{
  p = (enum Qt::NavigationMode)obj.toInt32();
}

void OrientationFromScriptValue(const QScriptValue &obj, enum Qt::Orientation &p)
{
  p = (enum Qt::Orientation)obj.toInt32();
}

void OrientationsFromScriptValue(const QScriptValue &obj, Qt::Orientations &p)
{
  p = (Qt::Orientations)obj.toInt32();
}

void PenCapStyleFromScriptValue(const QScriptValue &obj, enum Qt::PenCapStyle &p)
{
  p = (enum Qt::PenCapStyle)obj.toInt32();
}

void PenJoinStyleFromScriptValue(const QScriptValue &obj, enum Qt::PenJoinStyle &p)
{
  p = (enum Qt::PenJoinStyle)obj.toInt32();
}

void PenStyleFromScriptValue(const QScriptValue &obj, enum Qt::PenStyle &p)
{
  p = (enum Qt::PenStyle)obj.toInt32();
}

void ScreenOrientationFromScriptValue(const QScriptValue &obj, enum Qt::ScreenOrientation &p)
{
  p = (enum Qt::ScreenOrientation)obj.toInt32();
}

void ScreenOrientationsFromScriptValue(const QScriptValue &obj, Qt::ScreenOrientations &p)
{
  p = (Qt::ScreenOrientations)obj.toInt32();
}

void ScrollBarPolicyFromScriptValue(const QScriptValue &obj, enum Qt::ScrollBarPolicy &p)
{
  p = (enum Qt::ScrollBarPolicy)obj.toInt32();
}

void ScrollPhaseFromScriptValue(const QScriptValue &obj, enum Qt::ScrollPhase &p)
{
  p = (enum Qt::ScrollPhase)obj.toInt32();
}

void ShortcutContextFromScriptValue(const QScriptValue &obj, enum Qt::ShortcutContext &p)
{
  p = (enum Qt::ShortcutContext)obj.toInt32();
}

void SizeHintFromScriptValue(const QScriptValue &obj, enum Qt::SizeHint &p)
{
  p = (enum Qt::SizeHint)obj.toInt32();
}

void SizeModeFromScriptValue(const QScriptValue &obj, enum Qt::SizeMode &p)
{
  p = (enum Qt::SizeMode)obj.toInt32();
}

void SortOrderFromScriptValue(const QScriptValue &obj, enum Qt::SortOrder &p)
{
  p = (enum Qt::SortOrder)obj.toInt32();
}

void TabFocusBehaviorFromScriptValue(const QScriptValue &obj, enum Qt::TabFocusBehavior &p)
{
  p = (enum Qt::TabFocusBehavior)obj.toInt32();
}

void TextElideModeFromScriptValue(const QScriptValue &obj, enum Qt::TextElideMode &p)
{
  p = (enum Qt::TextElideMode)obj.toInt32();
}

void TextFlagFromScriptValue(const QScriptValue &obj, enum Qt::TextFlag &p)
{
  p = (enum Qt::TextFlag)obj.toInt32();
}

void TextFormatFromScriptValue(const QScriptValue &obj, enum Qt::TextFormat &p)
{
  p = (enum Qt::TextFormat)obj.toInt32();
}

void TextInteractionFlagFromScriptValue(const QScriptValue &obj, enum Qt::TextInteractionFlag &p)
{
  p = (enum Qt::TextInteractionFlag)obj.toInt32();
}

void TextInteractionFlagsFromScriptValue(const QScriptValue &obj, Qt::TextInteractionFlags &p)
{
  p = (Qt::TextInteractionFlags)obj.toInt32();
}

void TileRuleFromScriptValue(const QScriptValue &obj, enum Qt::TileRule &p)
{
  p = (enum Qt::TileRule)obj.toInt32();
}

void TimeSpecFromScriptValue(const QScriptValue &obj, enum Qt::TimeSpec &p)
{
  p = (enum Qt::TimeSpec)obj.toInt32();
}

void TimerTypeFromScriptValue(const QScriptValue &obj, enum Qt::TimerType &p)
{
  p = (enum Qt::TimerType)obj.toInt32();
}

void ToolBarAreaFromScriptValue(const QScriptValue &obj, enum Qt::ToolBarArea &p)
{
  p = (enum Qt::ToolBarArea)obj.toInt32();
}

void ToolBarAreasFromScriptValue(const QScriptValue &obj, Qt::ToolBarAreas &p)
{
  p = (Qt::ToolBarAreas)obj.toInt32();
}

void ToolButtonStyleFromScriptValue(const QScriptValue &obj, enum Qt::ToolButtonStyle &p)
{
  p = (enum Qt::ToolButtonStyle)obj.toInt32();
}

void TouchPointStateFromScriptValue(const QScriptValue &obj, enum Qt::TouchPointState &p)
{
  p = (enum Qt::TouchPointState)obj.toInt32();
}

void TouchPointStatesFromScriptValue(const QScriptValue &obj, Qt::TouchPointStates &p)
{
  p = (Qt::TouchPointStates)obj.toInt32();
}

void TransformationModeFromScriptValue(const QScriptValue &obj, enum Qt::TransformationMode &p)
{
  p = (enum Qt::TransformationMode)obj.toInt32();
}

void UIEffectFromScriptValue(const QScriptValue &obj, enum Qt::UIEffect &p)
{
  p = (enum Qt::UIEffect)obj.toInt32();
}

void WhiteSpaceModeFromScriptValue(const QScriptValue &obj, enum Qt::WhiteSpaceMode &p)
{
  p = (enum Qt::WhiteSpaceMode)obj.toInt32();
}

void WidgetAttributeFromScriptValue(const QScriptValue &obj, enum Qt::WidgetAttribute &p)
{
  p = (enum Qt::WidgetAttribute)obj.toInt32();
}

void WindowFlagsFromScriptValue(const QScriptValue &obj, Qt::WindowFlags &p)
{
  p = (Qt::WindowFlags)obj.toInt32();
}

void WindowFrameSectionFromScriptValue(const QScriptValue &obj, enum Qt::WindowFrameSection &p)
{
  p = (enum Qt::WindowFrameSection)obj.toInt32();
}

void WindowModalityFromScriptValue(const QScriptValue &obj, enum Qt::WindowModality &p)
{
  p = (enum Qt::WindowModality)obj.toInt32();
}

void WindowStateFromScriptValue(const QScriptValue &obj, enum Qt::WindowState &p)
{
  p = (enum Qt::WindowState)obj.toInt32();
}

void WindowStatesFromScriptValue(const QScriptValue &obj, Qt::WindowStates &p)
{
  p = (Qt::WindowStates)obj.toInt32();
}

void WindowTypeFromScriptValue(const QScriptValue &obj, enum Qt::WindowType &p)
{
  p = (enum Qt::WindowType)obj.toInt32();
}

////////////////////////////////////////////////////////////////////////////////

void setupQt(QScriptEngine *engine)
{
  QScriptValue widget = engine->newObject();
  QScriptValue::PropertyFlags ro = QScriptValue::ReadOnly | QScriptValue::Undeletable;

  qScriptRegisterMetaType(engine, AlignmentToScriptValue,     AlignmentFromScriptValue);
  qScriptRegisterMetaType(engine, AlignmentFlagToScriptValue, AlignmentFlagFromScriptValue);
  widget.setProperty("AlignLeft",            QScriptValue(engine, Qt::AlignLeft),            ro);
  widget.setProperty("AlignRight",           QScriptValue(engine, Qt::AlignRight),           ro);
  widget.setProperty("AlignHCenter",         QScriptValue(engine, Qt::AlignHCenter),         ro);
  widget.setProperty("AlignJustify",         QScriptValue(engine, Qt::AlignJustify),         ro);
  widget.setProperty("AlignTop",             QScriptValue(engine, Qt::AlignTop),             ro);
  widget.setProperty("AlignBottom",          QScriptValue(engine, Qt::AlignBottom),          ro);
  widget.setProperty("AlignVCenter",         QScriptValue(engine, Qt::AlignVCenter),         ro);
  widget.setProperty("AlignBaseline",        QScriptValue(engine, Qt::AlignBaseline),        ro);
  widget.setProperty("AlignCenter",          QScriptValue(engine, Qt::AlignCenter),          ro);
  widget.setProperty("AlignAbsolute",        QScriptValue(engine, Qt::AlignAbsolute),        ro);
  widget.setProperty("AlignLeading",         QScriptValue(engine, Qt::AlignLeading),         ro);
  widget.setProperty("AlignTrailing",        QScriptValue(engine, Qt::AlignTrailing),        ro);
  widget.setProperty("AlignHorizontal_Mask", QScriptValue(engine, Qt::AlignHorizontal_Mask), ro);
  widget.setProperty("AlignVertical_Mask",   QScriptValue(engine, Qt::AlignVertical_Mask),   ro);

  qScriptRegisterMetaType(engine, AnchorPointToScriptValue, AnchorPointFromScriptValue);
  widget.setProperty("AnchorLeft",             QScriptValue(engine, Qt::AnchorLeft),             ro);
  widget.setProperty("AnchorHorizontalCenter", QScriptValue(engine, Qt::AnchorHorizontalCenter), ro);
  widget.setProperty("AnchorRight",            QScriptValue(engine, Qt::AnchorRight),            ro);
  widget.setProperty("AnchorTop",              QScriptValue(engine, Qt::AnchorTop),              ro);
  widget.setProperty("AnchorVerticalCenter",   QScriptValue(engine, Qt::AnchorVerticalCenter),   ro);
  widget.setProperty("AnchorBottom",           QScriptValue(engine, Qt::AnchorBottom),           ro);

  qScriptRegisterMetaType(engine, ApplicationAttributeToScriptValue, ApplicationAttributeFromScriptValue);
  widget.setProperty("AA_DontShowIconsInMenus",                    QScriptValue(engine, Qt::AA_DontShowIconsInMenus),                    ro);
  widget.setProperty("AA_NativeWindows",                           QScriptValue(engine, Qt::AA_NativeWindows),                           ro);
  widget.setProperty("AA_DontCreateNativeWidgetSiblings",          QScriptValue(engine, Qt::AA_DontCreateNativeWidgetSiblings),          ro);
#if QT_VERSION >= 0x050700
  widget.setProperty("AA_PluginApplication",                       QScriptValue(engine, Qt::AA_PluginApplication),                       ro);
#endif
  widget.setProperty("AA_DontUseNativeMenuBar",                    QScriptValue(engine, Qt::AA_DontUseNativeMenuBar),                    ro);
  widget.setProperty("AA_MacDontSwapCtrlAndMeta",                  QScriptValue(engine, Qt::AA_MacDontSwapCtrlAndMeta),                  ro);
  widget.setProperty("AA_Use96Dpi",                                QScriptValue(engine, Qt::AA_Use96Dpi),                                ro);
  widget.setProperty("AA_SynthesizeTouchForUnhandledMouseEvents",  QScriptValue(engine, Qt::AA_SynthesizeTouchForUnhandledMouseEvents),  ro);
  widget.setProperty("AA_SynthesizeMouseForUnhandledTouchEvents",  QScriptValue(engine, Qt::AA_SynthesizeMouseForUnhandledTouchEvents),  ro);
  widget.setProperty("AA_UseHighDpiPixmaps",                       QScriptValue(engine, Qt::AA_UseHighDpiPixmaps),                       ro);
  widget.setProperty("AA_ForceRasterWidgets",                      QScriptValue(engine, Qt::AA_ForceRasterWidgets),                      ro);
  widget.setProperty("AA_UseDesktopOpenGL",                        QScriptValue(engine, Qt::AA_UseDesktopOpenGL),                        ro);
  widget.setProperty("AA_UseOpenGLES",                             QScriptValue(engine, Qt::AA_UseOpenGLES),                             ro);
  widget.setProperty("AA_UseSoftwareOpenGL",                       QScriptValue(engine, Qt::AA_UseSoftwareOpenGL),                       ro);
  widget.setProperty("AA_ShareOpenGLContexts",                     QScriptValue(engine, Qt::AA_ShareOpenGLContexts),                     ro);
  widget.setProperty("AA_SetPalette",                              QScriptValue(engine, Qt::AA_SetPalette),                              ro);
#if QT_VERSION >= 0x050600
  widget.setProperty("AA_EnableHighDpiScaling",                    QScriptValue(engine, Qt::AA_EnableHighDpiScaling),                    ro);
  widget.setProperty("AA_DisableHighDpiScaling",                   QScriptValue(engine, Qt::AA_DisableHighDpiScaling),                   ro);
#endif
#if QT_VERSION >= 0x050700
  widget.setProperty("AA_UseStyleSheetPropagationInWidgetStyles",  QScriptValue(engine, Qt::AA_UseStyleSheetPropagationInWidgetStyles),  ro);
  widget.setProperty("AA_DontUseNativeDialogs",                    QScriptValue(engine, Qt::AA_DontUseNativeDialogs),                    ro);
  widget.setProperty("AA_SynthesizeMouseForUnhandledTabletEvents", QScriptValue(engine, Qt::AA_SynthesizeMouseForUnhandledTabletEvents), ro);
  widget.setProperty("AA_CompressHighFrequencyEvents",             QScriptValue(engine, Qt::AA_CompressHighFrequencyEvents),             ro);
#endif
#if QT_VERSION >= 0x050800
  widget.setProperty("AA_DontCheckOpenGLContextThreadAffinity",    QScriptValue(engine, Qt::AA_DontCheckOpenGLContextThreadAffinity),    ro);
  widget.setProperty("AA_DisableShaderDiskCache",                  QScriptValue(engine, Qt::AA_DisableShaderDiskCache),                  ro);
#endif

  // deprecated, but how do we warn people about using them? {
  widget.setProperty("AA_ImmediateWidgetCreation",                 QScriptValue(engine, Qt::AA_ImmediateWidgetCreation),        ro);
  widget.setProperty("AA_MacPluginApplication",                    QScriptValue(engine, Qt::AA_MacPluginApplication),           ro);
  widget.setProperty("AA_MSWindowsUseDirect3DByDefault",           QScriptValue(engine, Qt::AA_MSWindowsUseDirect3DByDefault),  ro);
  widget.setProperty("AA_X11InitThreads",                          QScriptValue(engine, Qt::AA_X11InitThreads),                 ro);
  // }

  qScriptRegisterMetaType(engine, ApplicationStateToScriptValue,  ApplicationStateFromScriptValue);
  qScriptRegisterMetaType(engine, ApplicationStatesToScriptValue, ApplicationStatesFromScriptValue);
  widget.setProperty("ApplicationSuspended", QScriptValue(engine, Qt::ApplicationSuspended), ro);
  widget.setProperty("ApplicationHidden",    QScriptValue(engine, Qt::ApplicationHidden),    ro);
  widget.setProperty("ApplicationInactive",  QScriptValue(engine, Qt::ApplicationInactive),  ro);
  widget.setProperty("ApplicationActive",    QScriptValue(engine, Qt::ApplicationActive),    ro);

  qScriptRegisterMetaType(engine, ArrowTypeToScriptValue, ArrowTypeFromScriptValue);
  widget.setProperty("NoArrow",                    QScriptValue(engine, Qt::NoArrow),                    ro);
  widget.setProperty("UpArrow",                    QScriptValue(engine, Qt::UpArrow),                    ro);
  widget.setProperty("DownArrow",                  QScriptValue(engine, Qt::DownArrow),                  ro);
  widget.setProperty("LeftArrow",                  QScriptValue(engine, Qt::LeftArrow),                  ro);
  widget.setProperty("RightArrow",                 QScriptValue(engine, Qt::RightArrow),                 ro);

  qScriptRegisterMetaType(engine, AspectRatioModeToScriptValue, AspectRatioModeFromScriptValue);
  widget.setProperty("IgnoreAspectRatio",          QScriptValue(engine, Qt::IgnoreAspectRatio),          ro);
  widget.setProperty("KeepAspectRatio",            QScriptValue(engine, Qt::KeepAspectRatio),            ro);
  widget.setProperty("KeepAspectRatioByExpanding", QScriptValue(engine, Qt::KeepAspectRatioByExpanding), ro);

  qScriptRegisterMetaType(engine, AxisToScriptValue,  AxisFromScriptValue);
  widget.setProperty("XAxis",                   QScriptValue(engine, Qt::XAxis),                ro);
  widget.setProperty("YAxis",                   QScriptValue(engine, Qt::YAxis),                ro);
  widget.setProperty("ZAxis",                   QScriptValue(engine, Qt::ZAxis),                ro);

  qScriptRegisterMetaType(engine, BGModeToScriptValue, BGModeFromScriptValue);
  widget.setProperty("TransparentMode",         QScriptValue(engine, Qt::TransparentMode),      ro);
  widget.setProperty("OpaqueMode",              QScriptValue(engine, Qt::OpaqueMode),           ro);

  qScriptRegisterMetaType(engine, BrushStyleToScriptValue, BrushStyleFromScriptValue);
  widget.setProperty("NoBrush",                 QScriptValue(engine, Qt::NoBrush),              ro);
  widget.setProperty("SolidPattern",            QScriptValue(engine, Qt::SolidPattern),         ro);
  widget.setProperty("Dense1Pattern",           QScriptValue(engine, Qt::Dense1Pattern),        ro);
  widget.setProperty("Dense2Pattern",           QScriptValue(engine, Qt::Dense2Pattern),        ro);
  widget.setProperty("Dense3Pattern",           QScriptValue(engine, Qt::Dense3Pattern),        ro);
  widget.setProperty("Dense4Pattern",           QScriptValue(engine, Qt::Dense4Pattern),        ro);
  widget.setProperty("Dense5Pattern",           QScriptValue(engine, Qt::Dense5Pattern),        ro);
  widget.setProperty("Dense6Pattern",           QScriptValue(engine, Qt::Dense6Pattern),        ro);
  widget.setProperty("Dense7Pattern",           QScriptValue(engine, Qt::Dense7Pattern),        ro);
  widget.setProperty("HorPattern",              QScriptValue(engine, Qt::HorPattern),           ro);
  widget.setProperty("VerPattern",              QScriptValue(engine, Qt::VerPattern),           ro);
  widget.setProperty("CrossPattern",            QScriptValue(engine, Qt::CrossPattern),         ro);
  widget.setProperty("BDiagPattern",            QScriptValue(engine, Qt::BDiagPattern),         ro);
  widget.setProperty("FDiagPattern",            QScriptValue(engine, Qt::FDiagPattern),         ro);
  widget.setProperty("DiagCrossPattern",        QScriptValue(engine, Qt::DiagCrossPattern),     ro);
  widget.setProperty("LinearGradientPattern",   QScriptValue(engine, Qt::LinearGradientPattern),  ro);
  widget.setProperty("ConicalGradientPattern",  QScriptValue(engine, Qt::ConicalGradientPattern), ro);
  widget.setProperty("RadialGradientPattern",   QScriptValue(engine, Qt::RadialGradientPattern),  ro);
  widget.setProperty("TexturePattern",          QScriptValue(engine, Qt::TexturePattern),         ro);

  qScriptRegisterMetaType(engine, CaseSensitivityToScriptValue, CaseSensitivityFromScriptValue);
  widget.setProperty("CaseInsensitive",         QScriptValue(engine, Qt::CaseInsensitive),      ro);
  widget.setProperty("CaseSensitive",           QScriptValue(engine, Qt::CaseSensitive),        ro);

  qScriptRegisterMetaType(engine, CheckStateToScriptValue, CheckStateFromScriptValue);
  widget.setProperty("Unchecked",               QScriptValue(engine, Qt::Unchecked),            ro);
  widget.setProperty("PartiallyChecked",        QScriptValue(engine, Qt::PartiallyChecked),     ro);
  widget.setProperty("Checked",                 QScriptValue(engine, Qt::Checked),              ro);

#if QT_VERSION >= 0x050900
  qScriptRegisterMetaType(engine, ChecksumTypeToScriptValue, ChecksumTypeFromScriptValue);
  widget.setProperty("ChecksumIso3309",         QScriptValue(engine, Qt::ChecksumIso3309),      ro);
  widget.setProperty("ChecksumItuV41",          QScriptValue(engine, Qt::ChecksumItuV41),       ro);
#endif

  qScriptRegisterMetaType(engine, ClipOperationToScriptValue, ClipOperationFromScriptValue);
  widget.setProperty("NoClip",                  QScriptValue(engine, Qt::NoClip),               ro);
  widget.setProperty("ReplaceClip",             QScriptValue(engine, Qt::ReplaceClip),          ro);
  widget.setProperty("IntersectClip",           QScriptValue(engine, Qt::IntersectClip),        ro);

  qScriptRegisterMetaType(engine, ConnectionTypeToScriptValue, ConnectionTypeFromScriptValue);
  widget.setProperty("AutoConnection",           QScriptValue(engine, Qt::AutoConnection),           ro);
  widget.setProperty("DirectConnection",         QScriptValue(engine, Qt::DirectConnection),         ro);
  widget.setProperty("QueuedConnection",         QScriptValue(engine, Qt::QueuedConnection),         ro);
  widget.setProperty("BlockingQueuedConnection", QScriptValue(engine, Qt::BlockingQueuedConnection), ro);
  widget.setProperty("UniqueConnection",         QScriptValue(engine, Qt::UniqueConnection),         ro);

  qScriptRegisterMetaType(engine, ContextMenuPolicyToScriptValue, ContextMenuPolicyFromScriptValue);
  widget.setProperty("NoContextMenu",             QScriptValue(engine, Qt::NoContextMenu),          ro);
  widget.setProperty("PreventContextMenu",        QScriptValue(engine, Qt::PreventContextMenu),     ro);
  widget.setProperty("DefaultContextMenu",        QScriptValue(engine, Qt::DefaultContextMenu),     ro);
  widget.setProperty("ActionsContextMenu",        QScriptValue(engine, Qt::ActionsContextMenu),     ro);
  widget.setProperty("CustomContextMenu",         QScriptValue(engine, Qt::CustomContextMenu),      ro);

  qScriptRegisterMetaType(engine, CoordinateSystemToScriptValue, CoordinateSystemFromScriptValue);
  widget.setProperty("DeviceCoordinates",         QScriptValue(engine, Qt::DeviceCoordinates),      ro);
  widget.setProperty("LogicalCoordinates",        QScriptValue(engine, Qt::LogicalCoordinates),     ro);

  qScriptRegisterMetaType(engine, CornerToScriptValue, CornerFromScriptValue);
  widget.setProperty("TopLeftCorner",             QScriptValue(engine, Qt::TopLeftCorner),          ro);
  widget.setProperty("TopRightCorner",            QScriptValue(engine, Qt::TopRightCorner),         ro);
  widget.setProperty("BottomLeftCorner",          QScriptValue(engine, Qt::BottomLeftCorner),       ro);
  widget.setProperty("BottomRightCorner",         QScriptValue(engine, Qt::BottomRightCorner),      ro);

  qScriptRegisterMetaType(engine, CursorMoveStyleToScriptValue, CursorMoveStyleFromScriptValue);
  widget.setProperty("LogicalMoveStyle",          QScriptValue(engine, Qt::LogicalMoveStyle),       ro);
  widget.setProperty("VisualMoveStyle",           QScriptValue(engine, Qt::VisualMoveStyle),        ro);

  qScriptRegisterMetaType(engine, CursorShapeToScriptValue, CursorShapeFromScriptValue);
  widget.setProperty("ArrowCursor",               QScriptValue(engine, Qt::ArrowCursor),            ro);
  widget.setProperty("UpArrowCursor",             QScriptValue(engine, Qt::UpArrowCursor),          ro);
  widget.setProperty("CrossCursor",               QScriptValue(engine, Qt::CrossCursor),            ro);
  widget.setProperty("WaitCursor",                QScriptValue(engine, Qt::WaitCursor),             ro);
  widget.setProperty("IBeamCursor",               QScriptValue(engine, Qt::IBeamCursor),            ro);
  widget.setProperty("SizeVerCursor",             QScriptValue(engine, Qt::SizeVerCursor),          ro);
  widget.setProperty("SizeHorCursor",             QScriptValue(engine, Qt::SizeHorCursor),          ro);
  widget.setProperty("SizeBDiagCursor",           QScriptValue(engine, Qt::SizeBDiagCursor),        ro);
  widget.setProperty("SizeFDiagCursor",           QScriptValue(engine, Qt::SizeFDiagCursor),        ro);
  widget.setProperty("SizeAllCursor",             QScriptValue(engine, Qt::SizeAllCursor),          ro);
  widget.setProperty("BlankCursor",               QScriptValue(engine, Qt::BlankCursor),            ro);
  widget.setProperty("SplitVCursor",              QScriptValue(engine, Qt::SplitVCursor),           ro);
  widget.setProperty("SplitHCursor",              QScriptValue(engine, Qt::SplitHCursor),           ro);
  widget.setProperty("PointingHandCursor",        QScriptValue(engine, Qt::PointingHandCursor),     ro);
  widget.setProperty("ForbiddenCursor",           QScriptValue(engine, Qt::ForbiddenCursor),        ro);
  widget.setProperty("OpenHandCursor",            QScriptValue(engine, Qt::OpenHandCursor),         ro);
  widget.setProperty("ClosedHandCursor",          QScriptValue(engine, Qt::ClosedHandCursor),       ro);
  widget.setProperty("WhatsThisCursor",           QScriptValue(engine, Qt::WhatsThisCursor),        ro);
  widget.setProperty("BusyCursor",                QScriptValue(engine, Qt::BusyCursor),             ro);
  widget.setProperty("DragMoveCursor",            QScriptValue(engine, Qt::DragMoveCursor),         ro);
  widget.setProperty("DragCopyCursor",            QScriptValue(engine, Qt::DragCopyCursor),         ro);
  widget.setProperty("DragLinkCursor",            QScriptValue(engine, Qt::DragLinkCursor),         ro);
  widget.setProperty("BitmapCursor",              QScriptValue(engine, Qt::BitmapCursor),           ro);

  qScriptRegisterMetaType(engine, DateFormatToScriptValue, DateFormatFromScriptValue);
  widget.setProperty("TextDate",                  QScriptValue(engine, Qt::TextDate),               ro);
  widget.setProperty("ISODate",                   QScriptValue(engine, Qt::ISODate),                ro);
#if QT_VERSION >= 0x050900
  widget.setProperty("ISODateWithMs",             QScriptValue(engine, Qt::ISODateWithMs),          ro);
#endif
  widget.setProperty("SystemLocaleShortDate",     QScriptValue(engine, Qt::SystemLocaleShortDate),  ro);
  widget.setProperty("SystemLocaleLongDate",      QScriptValue(engine, Qt::SystemLocaleLongDate),   ro);
  widget.setProperty("DefaultLocaleShortDate",    QScriptValue(engine, Qt::DefaultLocaleShortDate), ro);
  widget.setProperty("DefaultLocaleLongDate",     QScriptValue(engine, Qt::DefaultLocaleLongDate),  ro);
  widget.setProperty("SystemLocaleDate",          QScriptValue(engine, Qt::SystemLocaleDate),       ro);
  widget.setProperty("LocaleDate",                QScriptValue(engine, Qt::LocaleDate),             ro);
  widget.setProperty("LocalDate",                 QScriptValue(engine, Qt::LocalDate),              ro);
  widget.setProperty("RFC2822Date",               QScriptValue(engine, Qt::RFC2822Date),            ro);

  qScriptRegisterMetaType(engine, DayOfWeekToScriptValue, DayOfWeekFromScriptValue);
  widget.setProperty("Monday",                    QScriptValue(engine, Qt::Monday),                 ro);
  widget.setProperty("Tuesday",                   QScriptValue(engine, Qt::Tuesday),                ro);
  widget.setProperty("Wednesday",                 QScriptValue(engine, Qt::Wednesday),              ro);
  widget.setProperty("Thursday",                  QScriptValue(engine, Qt::Thursday),               ro);
  widget.setProperty("Friday",                    QScriptValue(engine, Qt::Friday),                 ro);
  widget.setProperty("Saturday",                  QScriptValue(engine, Qt::Saturday),               ro);
  widget.setProperty("Sunday",                    QScriptValue(engine, Qt::Sunday),                 ro);

  qScriptRegisterMetaType(engine, DockWidgetAreaToScriptValue, DockWidgetAreaFromScriptValue);
  widget.setProperty("LeftDockWidgetArea",        QScriptValue(engine, Qt::LeftDockWidgetArea),     ro);
  widget.setProperty("RightDockWidgetArea",       QScriptValue(engine, Qt::RightDockWidgetArea),    ro);
  widget.setProperty("TopDockWidgetArea",         QScriptValue(engine, Qt::TopDockWidgetArea),      ro);
  widget.setProperty("BottomDockWidgetArea",      QScriptValue(engine, Qt::BottomDockWidgetArea),   ro);
  widget.setProperty("AllDockWidgetAreas",        QScriptValue(engine, Qt::AllDockWidgetAreas),     ro);
  widget.setProperty("NoDockWidgetArea",          QScriptValue(engine, Qt::NoDockWidgetArea),       ro);

  qScriptRegisterMetaType(engine, DropActionToScriptValue,  DropActionFromScriptValue);
  qScriptRegisterMetaType(engine, DropActionsToScriptValue, DropActionsFromScriptValue);
  widget.setProperty("CopyAction",                QScriptValue(engine, Qt::CopyAction),             ro);
  widget.setProperty("MoveAction",                QScriptValue(engine, Qt::MoveAction),             ro);
  widget.setProperty("LinkAction",                QScriptValue(engine, Qt::LinkAction),             ro);
  widget.setProperty("ActionMask",                QScriptValue(engine, Qt::ActionMask),             ro);
  widget.setProperty("IgnoreAction",              QScriptValue(engine, Qt::IgnoreAction),           ro);
  widget.setProperty("TargetMoveAction",          QScriptValue(engine, Qt::TargetMoveAction),       ro);

  qScriptRegisterMetaType(engine, EdgeToScriptValue,  EdgeFromScriptValue);
  qScriptRegisterMetaType(engine, EdgesToScriptValue, EdgesFromScriptValue);
  widget.setProperty("TopEdge",                   QScriptValue(engine, Qt::TopEdge),                ro);
  widget.setProperty("LeftEdge",                  QScriptValue(engine, Qt::LeftEdge),               ro);
  widget.setProperty("RightEdge",                 QScriptValue(engine, Qt::RightEdge),              ro);
  widget.setProperty("BottomEdge",                QScriptValue(engine, Qt::BottomEdge),             ro);

#if QT_VERSION >= 0x050600
  qScriptRegisterMetaType(engine, EnterKeyTypeToScriptValue, EnterKeyTypeFromScriptValue);
  widget.setProperty("EnterKeyDefault",           QScriptValue(engine, Qt::EnterKeyDefault),  ro);
  widget.setProperty("EnterKeyReturn",            QScriptValue(engine, Qt::EnterKeyReturn),   ro);
  widget.setProperty("EnterKeyDone",              QScriptValue(engine, Qt::EnterKeyDone),     ro);
  widget.setProperty("EnterKeyGo",                QScriptValue(engine, Qt::EnterKeyGo),       ro);
  widget.setProperty("EnterKeySend",              QScriptValue(engine, Qt::EnterKeySend),     ro);
  widget.setProperty("EnterKeySearch",            QScriptValue(engine, Qt::EnterKeySearch),   ro);
  widget.setProperty("EnterKeyNext",              QScriptValue(engine, Qt::EnterKeyNext),     ro);
  widget.setProperty("EnterKeyPrevious",          QScriptValue(engine, Qt::EnterKeyPrevious), ro);
#endif

  qScriptRegisterMetaType(engine, EventPriorityToScriptValue, EventPriorityFromScriptValue);
  widget.setProperty("HighEventPriority",         QScriptValue(engine, Qt::HighEventPriority),      ro);
  widget.setProperty("NormalEventPriority",       QScriptValue(engine, Qt::NormalEventPriority),    ro);
  widget.setProperty("LowEventPriority",          QScriptValue(engine, Qt::LowEventPriority),       ro);

  qScriptRegisterMetaType(engine, FillRuleToScriptValue, FillRuleFromScriptValue);
  widget.setProperty("OddEvenFill",               QScriptValue(engine, Qt::OddEvenFill),            ro);
  widget.setProperty("WindingFill",               QScriptValue(engine, Qt::WindingFill),            ro);

  qScriptRegisterMetaType(engine,  FindChildOptionToScriptValue,  FindChildOptionFromScriptValue);
  qScriptRegisterMetaType(engine,  FindChildOptionsToScriptValue, FindChildOptionsFromScriptValue);
  widget.setProperty("FindDirectChildrenOnly",    QScriptValue(engine, Qt::FindDirectChildrenOnly), ro);
  widget.setProperty("FindChildrenRecursively",   QScriptValue(engine, Qt::FindChildrenRecursively),ro);

  qScriptRegisterMetaType(engine,  FocusPolicyToScriptValue,  FocusPolicyFromScriptValue);
  widget.setProperty("TabFocus",                  QScriptValue(engine, Qt::TabFocus),               ro);
  widget.setProperty("ClickFocus",                QScriptValue(engine, Qt::ClickFocus),             ro);
  widget.setProperty("StrongFocus",               QScriptValue(engine, Qt::StrongFocus),            ro);
  widget.setProperty("WheelFocus",                QScriptValue(engine, Qt::WheelFocus),             ro);
  widget.setProperty("NoFocus",                   QScriptValue(engine, Qt::NoFocus),                ro);

  qScriptRegisterMetaType(engine, FocusReasonToScriptValue, FocusReasonFromScriptValue);
  widget.setProperty("MouseFocusReason",          QScriptValue(engine, Qt::MouseFocusReason),       ro);
  widget.setProperty("TabFocusReason",            QScriptValue(engine, Qt::TabFocusReason),         ro);
  widget.setProperty("BacktabFocusReason",        QScriptValue(engine, Qt::BacktabFocusReason),     ro);
  widget.setProperty("ActiveWindowFocusReason",   QScriptValue(engine, Qt::ActiveWindowFocusReason),ro);
  widget.setProperty("PopupFocusReason",          QScriptValue(engine, Qt::PopupFocusReason),       ro);
  widget.setProperty("ShortcutFocusReason",       QScriptValue(engine, Qt::ShortcutFocusReason),    ro);
  widget.setProperty("MenuBarFocusReason",        QScriptValue(engine, Qt::MenuBarFocusReason),     ro);
  widget.setProperty("OtherFocusReason",          QScriptValue(engine, Qt::OtherFocusReason),       ro);

  qScriptRegisterMetaType(engine, GestureFlagToScriptValue,  GestureFlagFromScriptValue);
  qScriptRegisterMetaType(engine, GestureFlagsToScriptValue, GestureFlagsFromScriptValue);
  widget.setProperty("DontStartGestureOnChildren",       QScriptValue(engine, Qt::DontStartGestureOnChildren),       ro);
  widget.setProperty("ReceivePartialGestures",           QScriptValue(engine, Qt::ReceivePartialGestures),           ro);
  widget.setProperty("IgnoredGesturesPropagateToParent", QScriptValue(engine, Qt::IgnoredGesturesPropagateToParent), ro);

  qScriptRegisterMetaType(engine, GestureStateToScriptValue, GestureStateFromScriptValue);
  widget.setProperty("NoGesture",                 QScriptValue(engine, Qt::NoGesture),              ro);
  widget.setProperty("GestureStarted",            QScriptValue(engine, Qt::GestureStarted),         ro);
  widget.setProperty("GestureUpdated",            QScriptValue(engine, Qt::GestureUpdated),         ro);
  widget.setProperty("GestureFinished",           QScriptValue(engine, Qt::GestureFinished),        ro);
  widget.setProperty("GestureCanceled",           QScriptValue(engine, Qt::GestureCanceled),        ro);

  qScriptRegisterMetaType(engine, GestureTypeToScriptValue, GestureTypeFromScriptValue);
  widget.setProperty("TapGesture",                QScriptValue(engine, Qt::TapGesture),             ro);
  widget.setProperty("TapAndHoldGesture",         QScriptValue(engine, Qt::TapAndHoldGesture),      ro);
  widget.setProperty("PanGesture",                QScriptValue(engine, Qt::PanGesture),             ro);
  widget.setProperty("PinchGesture",              QScriptValue(engine, Qt::PinchGesture),           ro);
  widget.setProperty("SwipeGesture",              QScriptValue(engine, Qt::SwipeGesture),           ro);
  widget.setProperty("CustomGesture",             QScriptValue(engine, Qt::CustomGesture),          ro);

  qScriptRegisterMetaType(engine, GlobalColorToScriptValue, GlobalColorFromScriptValue);
  widget.setProperty("white",                     QScriptValue(engine, Qt::white),                  ro);
  widget.setProperty("black",                     QScriptValue(engine, Qt::black),                  ro);
  widget.setProperty("red",                       QScriptValue(engine, Qt::red),                    ro);
  widget.setProperty("darkRed",                   QScriptValue(engine, Qt::darkRed),                ro);
  widget.setProperty("green",                     QScriptValue(engine, Qt::green),                  ro);
  widget.setProperty("darkGreen",                 QScriptValue(engine, Qt::darkGreen),              ro);
  widget.setProperty("blue",                      QScriptValue(engine, Qt::blue),                   ro);
  widget.setProperty("darkBlue",                  QScriptValue(engine, Qt::darkBlue),               ro);
  widget.setProperty("cyan",                      QScriptValue(engine, Qt::cyan),                   ro);
  widget.setProperty("darkCyan",                  QScriptValue(engine, Qt::darkCyan),               ro);
  widget.setProperty("magenta",                   QScriptValue(engine, Qt::magenta),                ro);
  widget.setProperty("darkMagenta",               QScriptValue(engine, Qt::darkMagenta),            ro);
  widget.setProperty("yellow",                    QScriptValue(engine, Qt::yellow),                 ro);
  widget.setProperty("darkYellow",                QScriptValue(engine, Qt::darkYellow),             ro);
  widget.setProperty("gray",                      QScriptValue(engine, Qt::gray),                   ro);
  widget.setProperty("darkGray",                  QScriptValue(engine, Qt::darkGray),               ro);
  widget.setProperty("lightGray",                 QScriptValue(engine, Qt::lightGray),              ro);
  widget.setProperty("transparent",               QScriptValue(engine, Qt::transparent),            ro);
  widget.setProperty("color0",                    QScriptValue(engine, Qt::color0),                 ro);
  widget.setProperty("color1",                    QScriptValue(engine, Qt::color1),                 ro);

  qScriptRegisterMetaType(engine, HitTestAccuracyToScriptValue, HitTestAccuracyFromScriptValue);
  widget.setProperty("ExactHit",                  QScriptValue(engine, Qt::ExactHit),               ro);
  widget.setProperty("FuzzyHit",                  QScriptValue(engine, Qt::FuzzyHit),               ro);

  qScriptRegisterMetaType(engine, ImageConversionFlagToScriptValue,  ImageConversionFlagFromScriptValue);
  qScriptRegisterMetaType(engine, ImageConversionFlagsToScriptValue, ImageConversionFlagsFromScriptValue);
  widget.setProperty("AutoColor",                 QScriptValue(engine, Qt::AutoColor),              ro);
  widget.setProperty("ColorOnly",                 QScriptValue(engine, Qt::ColorOnly),              ro);
  widget.setProperty("MonoOnly",                  QScriptValue(engine, Qt::MonoOnly),               ro);
  widget.setProperty("DiffuseDither",             QScriptValue(engine, Qt::DiffuseDither),          ro);
  widget.setProperty("OrderedDither",             QScriptValue(engine, Qt::OrderedDither),          ro);
  widget.setProperty("ThresholdDither",           QScriptValue(engine, Qt::ThresholdDither),        ro);
  widget.setProperty("ThresholdAlphaDither",      QScriptValue(engine, Qt::ThresholdAlphaDither),   ro);
  widget.setProperty("OrderedAlphaDither",        QScriptValue(engine, Qt::OrderedAlphaDither),     ro);
  widget.setProperty("DiffuseAlphaDither",        QScriptValue(engine, Qt::DiffuseAlphaDither),     ro);
  widget.setProperty("PreferDither",              QScriptValue(engine, Qt::PreferDither),           ro);
  widget.setProperty("AvoidDither",               QScriptValue(engine, Qt::AvoidDither),            ro);
  widget.setProperty("AutoDither",                QScriptValue(engine, Qt::AutoDither),             ro);
  widget.setProperty("NoOpaqueDetection",         QScriptValue(engine, Qt::NoOpaqueDetection),      ro);
  widget.setProperty("NoFormatConversion",        QScriptValue(engine, Qt::NoFormatConversion),     ro);

  qScriptRegisterMetaType(engine, InputMethodHintToScriptValue,  InputMethodHintFromScriptValue);
  qScriptRegisterMetaType(engine, InputMethodHintsToScriptValue, InputMethodHintsFromScriptValue);
  widget.setProperty("ImhNone",                   QScriptValue(engine, Qt::ImhNone),                   ro);
  widget.setProperty("ImhHiddenText",             QScriptValue(engine, Qt::ImhHiddenText),             ro);
  widget.setProperty("ImhSensitiveData",          QScriptValue(engine, Qt::ImhSensitiveData),          ro);
  widget.setProperty("ImhNoAutoUppercase",        QScriptValue(engine, Qt::ImhNoAutoUppercase),        ro);
  widget.setProperty("ImhPreferNumbers",          QScriptValue(engine, Qt::ImhPreferNumbers),          ro);
  widget.setProperty("ImhPreferUppercase",        QScriptValue(engine, Qt::ImhPreferUppercase),        ro);
  widget.setProperty("ImhPreferLowercase",        QScriptValue(engine, Qt::ImhPreferLowercase),        ro);
  widget.setProperty("ImhNoPredictiveText",       QScriptValue(engine, Qt::ImhNoPredictiveText),       ro);
  widget.setProperty("ImhDate",                   QScriptValue(engine, Qt::ImhDate),                   ro);
  widget.setProperty("ImhTime",                   QScriptValue(engine, Qt::ImhTime),                   ro);
  widget.setProperty("ImhPreferLatin",            QScriptValue(engine, Qt::ImhPreferLatin),            ro);
  widget.setProperty("ImhMultiLine",              QScriptValue(engine, Qt::ImhMultiLine),              ro);
  widget.setProperty("ImhDigitsOnly",             QScriptValue(engine, Qt::ImhDigitsOnly),             ro);
  widget.setProperty("ImhFormattedNumbersOnly",   QScriptValue(engine, Qt::ImhFormattedNumbersOnly),   ro);
  widget.setProperty("ImhUppercaseOnly",          QScriptValue(engine, Qt::ImhUppercaseOnly),          ro);
  widget.setProperty("ImhLowercaseOnly",          QScriptValue(engine, Qt::ImhLowercaseOnly),          ro);
  widget.setProperty("ImhDialableCharactersOnly", QScriptValue(engine, Qt::ImhDialableCharactersOnly), ro);
  widget.setProperty("ImhEmailCharactersOnly",    QScriptValue(engine, Qt::ImhEmailCharactersOnly),    ro);
  widget.setProperty("ImhUrlCharactersOnly",      QScriptValue(engine, Qt::ImhUrlCharactersOnly),      ro);
  widget.setProperty("ImhLatinOnly",              QScriptValue(engine, Qt::ImhLatinOnly),              ro);
  widget.setProperty("ImhExclusiveInputMask",     QScriptValue(engine, Qt::ImhExclusiveInputMask),     ro);

  qScriptRegisterMetaType(engine, InputMethodQueriesToScriptValue, InputMethodQueriesFromScriptValue);
  qScriptRegisterMetaType(engine, InputMethodQueryToScriptValue,   InputMethodQueryFromScriptValue);
  widget.setProperty("ImEnabled",                 QScriptValue(engine, Qt::ImEnabled),                 ro);
  widget.setProperty("ImMicroFocus",              QScriptValue(engine, Qt::ImMicroFocus),              ro);
  widget.setProperty("ImCursorRectangle",         QScriptValue(engine, Qt::ImCursorRectangle),         ro);
  widget.setProperty("ImFont",                    QScriptValue(engine, Qt::ImFont),                    ro);
  widget.setProperty("ImCursorPosition",          QScriptValue(engine, Qt::ImCursorPosition),          ro);
  widget.setProperty("ImSurroundingText",         QScriptValue(engine, Qt::ImSurroundingText),         ro);
  widget.setProperty("ImCurrentSelection",        QScriptValue(engine, Qt::ImCurrentSelection),        ro);

  widget.setProperty("ImMaximumTextLength",       QScriptValue(engine, Qt::ImMaximumTextLength),       ro);
  widget.setProperty("ImAnchorPosition",          QScriptValue(engine, Qt::ImAnchorPosition),          ro);
  widget.setProperty("ImHints",                   QScriptValue(engine, Qt::ImHints),                   ro);
  widget.setProperty("ImPreferredLanguage",       QScriptValue(engine, Qt::ImPreferredLanguage),       ro);
  widget.setProperty("ImPlatformData",            QScriptValue(engine, Qt::ImPlatformData),            ro);
  widget.setProperty("ImAbsolutePosition",        QScriptValue(engine, Qt::ImAbsolutePosition),        ro);
  widget.setProperty("ImTextBeforeCursor",        QScriptValue(engine, Qt::ImTextBeforeCursor),        ro);
  widget.setProperty("ImTextAfterCursor",         QScriptValue(engine, Qt::ImTextAfterCursor),         ro);
#if QT_VERSION >= 0x050700
  widget.setProperty("ImEnterKeyType",            QScriptValue(engine, Qt::ImEnterKeyType),            ro);
  widget.setProperty("ImAnchorRectangle",         QScriptValue(engine, Qt::ImAnchorRectangle),         ro);
  widget.setProperty("ImInputItemClipRectangle",  QScriptValue(engine, Qt::ImInputItemClipRectangle),  ro);
#endif
  widget.setProperty("ImQueryInput",              QScriptValue(engine, Qt::ImQueryInput),              ro);
  widget.setProperty("ImQueryAll",                QScriptValue(engine, Qt::ImQueryAll),                ro);

  qScriptRegisterMetaType(engine, ItemDataRoleToScriptValue, ItemDataRoleFromScriptValue);
  widget.setProperty("DisplayRole",               QScriptValue(engine, Qt::DisplayRole),            ro);
  widget.setProperty("DecorationRole",            QScriptValue(engine, Qt::DecorationRole),         ro);
  widget.setProperty("EditRole",                  QScriptValue(engine, Qt::EditRole),               ro);
  widget.setProperty("ToolTipRole",               QScriptValue(engine, Qt::ToolTipRole),            ro);
  widget.setProperty("StatusTipRole",             QScriptValue(engine, Qt::StatusTipRole),          ro);
  widget.setProperty("WhatsThisRole",             QScriptValue(engine, Qt::WhatsThisRole),          ro);
  widget.setProperty("SizeHintRole",              QScriptValue(engine, Qt::SizeHintRole),           ro);
  widget.setProperty("FontRole",                  QScriptValue(engine, Qt::FontRole),               ro);
  widget.setProperty("TextAlignmentRole",         QScriptValue(engine, Qt::TextAlignmentRole),      ro);
  widget.setProperty("BackgroundRole",            QScriptValue(engine, Qt::BackgroundRole),         ro);
  widget.setProperty("BackgroundColorRole",       QScriptValue(engine, Qt::BackgroundColorRole),    ro);
  widget.setProperty("ForegroundRole",            QScriptValue(engine, Qt::ForegroundRole),         ro);
  widget.setProperty("TextColorRole",             QScriptValue(engine, Qt::TextColorRole),          ro);
  widget.setProperty("CheckStateRole",            QScriptValue(engine, Qt::CheckStateRole),         ro);
  widget.setProperty("InitialSortOrderRole",      QScriptValue(engine, Qt::InitialSortOrderRole),   ro);
  widget.setProperty("AccessibleTextRole",        QScriptValue(engine, Qt::AccessibleTextRole),     ro);
  widget.setProperty("AccessibleDescriptionRole", QScriptValue(engine, Qt::AccessibleDescriptionRole), ro);
  widget.setProperty("UserRole",                  QScriptValue(engine, Qt::UserRole),               ro);

  qScriptRegisterMetaType(engine, ItemFlagToScriptValue,  ItemFlagFromScriptValue);
  qScriptRegisterMetaType(engine, ItemFlagsToScriptValue, ItemFlagsFromScriptValue);
  widget.setProperty("NoItemFlags",               QScriptValue(engine, Qt::NoItemFlags),            ro);
  widget.setProperty("ItemIsSelectable",          QScriptValue(engine, Qt::ItemIsSelectable),       ro);
  widget.setProperty("ItemIsEditable",            QScriptValue(engine, Qt::ItemIsEditable),         ro);
  widget.setProperty("ItemIsDragEnabled",         QScriptValue(engine, Qt::ItemIsDragEnabled),      ro);
  widget.setProperty("ItemIsDropEnabled",         QScriptValue(engine, Qt::ItemIsDropEnabled),      ro);
  widget.setProperty("ItemIsUserCheckable",       QScriptValue(engine, Qt::ItemIsUserCheckable),    ro);
  widget.setProperty("ItemIsEnabled",             QScriptValue(engine, Qt::ItemIsEnabled),          ro);
#if QT_VERSION >= 0x050900
  widget.setProperty("ItemIsAutoTristate",        QScriptValue(engine, Qt::ItemIsAutoTristate),     ro);
#endif
  widget.setProperty("ItemIsTristate",            QScriptValue(engine, Qt::ItemIsTristate),         ro);
  widget.setProperty("ItemNeverHasChildren",      QScriptValue(engine, Qt::ItemNeverHasChildren),   ro);
  widget.setProperty("ItemIsUserTristate",        QScriptValue(engine, Qt::ItemIsUserTristate),     ro);

  qScriptRegisterMetaType(engine, ItemSelectionModeToScriptValue, ItemSelectionModeFromScriptValue);
  widget.setProperty("ContainsItemShape",          QScriptValue(engine, Qt::ContainsItemShape),          ro);
  widget.setProperty("IntersectsItemShape",        QScriptValue(engine, Qt::IntersectsItemShape),        ro);
  widget.setProperty("ContainsItemBoundingRect",   QScriptValue(engine, Qt::ContainsItemBoundingRect),   ro);
  widget.setProperty("IntersectsItemBoundingRect", QScriptValue(engine, Qt::IntersectsItemBoundingRect), ro);

  qScriptRegisterMetaType(engine, ItemSelectionOperationToScriptValue, ItemSelectionOperationFromScriptValue);
  widget.setProperty("ReplaceSelection", QScriptValue(engine, Qt::ReplaceSelection), ro);
  widget.setProperty("AddToSelection",   QScriptValue(engine, Qt::AddToSelection),   ro);

  qScriptRegisterMetaType(engine, KeyToScriptValue, KeyFromScriptValue);
  widget.setProperty("Key_Escape",                QScriptValue(engine, Qt::Key_Escape),             ro);
  widget.setProperty("Key_Tab",                   QScriptValue(engine, Qt::Key_Tab),                ro);
  widget.setProperty("Key_Backtab",               QScriptValue(engine, Qt::Key_Backtab),            ro);
  widget.setProperty("Key_Backspace",             QScriptValue(engine, Qt::Key_Backspace),          ro);
  widget.setProperty("Key_Return",                QScriptValue(engine, Qt::Key_Return),             ro);
  widget.setProperty("Key_Enter",                 QScriptValue(engine, Qt::Key_Enter),              ro);
  widget.setProperty("Key_Insert",                QScriptValue(engine, Qt::Key_Insert),             ro);
  widget.setProperty("Key_Delete",                QScriptValue(engine, Qt::Key_Delete),             ro);
  widget.setProperty("Key_Pause",                 QScriptValue(engine, Qt::Key_Pause),              ro);
  widget.setProperty("Key_Print",                 QScriptValue(engine, Qt::Key_Print),              ro);
  widget.setProperty("Key_SysReq",                QScriptValue(engine, Qt::Key_SysReq),             ro);
  widget.setProperty("Key_Clear",                 QScriptValue(engine, Qt::Key_Clear),              ro);
  widget.setProperty("Key_Home",                  QScriptValue(engine, Qt::Key_Home),               ro);
  widget.setProperty("Key_End",                   QScriptValue(engine, Qt::Key_End),                ro);
  widget.setProperty("Key_Left",                  QScriptValue(engine, Qt::Key_Left),               ro);
  widget.setProperty("Key_Up",                    QScriptValue(engine, Qt::Key_Up),                 ro);
  widget.setProperty("Key_Right",                 QScriptValue(engine, Qt::Key_Right),              ro);
  widget.setProperty("Key_Down",                  QScriptValue(engine, Qt::Key_Down),               ro);
  widget.setProperty("Key_PageUp",                QScriptValue(engine, Qt::Key_PageUp),             ro);
  widget.setProperty("Key_PageDown",              QScriptValue(engine, Qt::Key_PageDown),           ro);
  widget.setProperty("Key_Shift",                 QScriptValue(engine, Qt::Key_Shift),              ro);
  widget.setProperty("Key_Control",               QScriptValue(engine, Qt::Key_Control),            ro);
  widget.setProperty("Key_Meta",                  QScriptValue(engine, Qt::Key_Meta),               ro);
  widget.setProperty("Key_Alt",                   QScriptValue(engine, Qt::Key_Alt),                ro);
  widget.setProperty("Key_AltGr",                 QScriptValue(engine, Qt::Key_AltGr),              ro);
  widget.setProperty("Key_CapsLock",              QScriptValue(engine, Qt::Key_CapsLock),           ro);
  widget.setProperty("Key_NumLock",               QScriptValue(engine, Qt::Key_NumLock),            ro);
  widget.setProperty("Key_ScrollLock",            QScriptValue(engine, Qt::Key_ScrollLock),         ro);
  widget.setProperty("Key_F1",                    QScriptValue(engine, Qt::Key_F1),                 ro);
  widget.setProperty("Key_F2",                    QScriptValue(engine, Qt::Key_F2),                 ro);
  widget.setProperty("Key_F3",                    QScriptValue(engine, Qt::Key_F3),                 ro);
  widget.setProperty("Key_F4",                    QScriptValue(engine, Qt::Key_F4),                 ro);
  widget.setProperty("Key_F5",                    QScriptValue(engine, Qt::Key_F5),                 ro);
  widget.setProperty("Key_F6",                    QScriptValue(engine, Qt::Key_F6),                 ro);
  widget.setProperty("Key_F7",                    QScriptValue(engine, Qt::Key_F7),                 ro);
  widget.setProperty("Key_F8",                    QScriptValue(engine, Qt::Key_F8),                 ro);
  widget.setProperty("Key_F9",                    QScriptValue(engine, Qt::Key_F9),                 ro);
  widget.setProperty("Key_F10",                   QScriptValue(engine, Qt::Key_F10),                ro);
  widget.setProperty("Key_F11",                   QScriptValue(engine, Qt::Key_F11),                ro);
  widget.setProperty("Key_F12",                   QScriptValue(engine, Qt::Key_F12),                ro);
  widget.setProperty("Key_F13",                   QScriptValue(engine, Qt::Key_F13),                ro);
  widget.setProperty("Key_F14",                   QScriptValue(engine, Qt::Key_F14),                ro);
  widget.setProperty("Key_F15",                   QScriptValue(engine, Qt::Key_F15),                ro);
  widget.setProperty("Key_F16",                   QScriptValue(engine, Qt::Key_F16),                ro);
  widget.setProperty("Key_F17",                   QScriptValue(engine, Qt::Key_F17),                ro);
  widget.setProperty("Key_F18",                   QScriptValue(engine, Qt::Key_F18),                ro);
  widget.setProperty("Key_F19",                   QScriptValue(engine, Qt::Key_F19),                ro);
  widget.setProperty("Key_F20",                   QScriptValue(engine, Qt::Key_F20),                ro);
  widget.setProperty("Key_F21",                   QScriptValue(engine, Qt::Key_F21),                ro);
  widget.setProperty("Key_F22",                   QScriptValue(engine, Qt::Key_F22),                ro);
  widget.setProperty("Key_F23",                   QScriptValue(engine, Qt::Key_F23),                ro);
  widget.setProperty("Key_F24",                   QScriptValue(engine, Qt::Key_F24),                ro);
  widget.setProperty("Key_F25",                   QScriptValue(engine, Qt::Key_F25),                ro);
  widget.setProperty("Key_F26",                   QScriptValue(engine, Qt::Key_F26),                ro);
  widget.setProperty("Key_F27",                   QScriptValue(engine, Qt::Key_F27),                ro);
  widget.setProperty("Key_F28",                   QScriptValue(engine, Qt::Key_F28),                ro);
  widget.setProperty("Key_F29",                   QScriptValue(engine, Qt::Key_F29),                ro);
  widget.setProperty("Key_F30",                   QScriptValue(engine, Qt::Key_F30),                ro);
  widget.setProperty("Key_F31",                   QScriptValue(engine, Qt::Key_F31),                ro);
  widget.setProperty("Key_F32",                   QScriptValue(engine, Qt::Key_F32),                ro);
  widget.setProperty("Key_F33",                   QScriptValue(engine, Qt::Key_F33),                ro);
  widget.setProperty("Key_F34",                   QScriptValue(engine, Qt::Key_F34),                ro);
  widget.setProperty("Key_F35",                   QScriptValue(engine, Qt::Key_F35),                ro);
  widget.setProperty("Key_Super_L",               QScriptValue(engine, Qt::Key_Super_L),            ro);
  widget.setProperty("Key_Super_R",               QScriptValue(engine, Qt::Key_Super_R),            ro);
  widget.setProperty("Key_Menu",                  QScriptValue(engine, Qt::Key_Menu),               ro);
  widget.setProperty("Key_Hyper_L",               QScriptValue(engine, Qt::Key_Hyper_L),            ro);
  widget.setProperty("Key_Hyper_R",               QScriptValue(engine, Qt::Key_Hyper_R),            ro);
  widget.setProperty("Key_Help",                  QScriptValue(engine, Qt::Key_Help),               ro);
  widget.setProperty("Key_Direction_L",           QScriptValue(engine, Qt::Key_Direction_L),        ro);
  widget.setProperty("Key_Direction_R",           QScriptValue(engine, Qt::Key_Direction_R),        ro);
  widget.setProperty("Key_Space",                 QScriptValue(engine, Qt::Key_Space),              ro);
  widget.setProperty("Key_Any",                   QScriptValue(engine, Qt::Key_Any),                ro);
  widget.setProperty("Key_Exclam",                QScriptValue(engine, Qt::Key_Exclam),             ro);
  widget.setProperty("Key_QuoteDbl",              QScriptValue(engine, Qt::Key_QuoteDbl),           ro);
  widget.setProperty("Key_NumberSign",            QScriptValue(engine, Qt::Key_NumberSign),         ro);
  widget.setProperty("Key_Dollar",                QScriptValue(engine, Qt::Key_Dollar),             ro);
  widget.setProperty("Key_Percent",               QScriptValue(engine, Qt::Key_Percent),            ro);
  widget.setProperty("Key_Ampersand",             QScriptValue(engine, Qt::Key_Ampersand),          ro);
  widget.setProperty("Key_Apostrophe",            QScriptValue(engine, Qt::Key_Apostrophe),         ro);
  widget.setProperty("Key_ParenLeft",             QScriptValue(engine, Qt::Key_ParenLeft),          ro);
  widget.setProperty("Key_ParenRight",            QScriptValue(engine, Qt::Key_ParenRight),         ro);
  widget.setProperty("Key_Asterisk",              QScriptValue(engine, Qt::Key_Asterisk),           ro);
  widget.setProperty("Key_Plus",                  QScriptValue(engine, Qt::Key_Plus),               ro);
  widget.setProperty("Key_Comma",                 QScriptValue(engine, Qt::Key_Comma),              ro);
  widget.setProperty("Key_Minus",                 QScriptValue(engine, Qt::Key_Minus),              ro);
  widget.setProperty("Key_Period",                QScriptValue(engine, Qt::Key_Period),             ro);
  widget.setProperty("Key_Slash",                 QScriptValue(engine, Qt::Key_Slash),              ro);
  widget.setProperty("Key_0",                     QScriptValue(engine, Qt::Key_0),                  ro);
  widget.setProperty("Key_1",                     QScriptValue(engine, Qt::Key_1),                  ro);
  widget.setProperty("Key_2",                     QScriptValue(engine, Qt::Key_2),                  ro);
  widget.setProperty("Key_3",                     QScriptValue(engine, Qt::Key_3),                  ro);
  widget.setProperty("Key_4",                     QScriptValue(engine, Qt::Key_4),                  ro);
  widget.setProperty("Key_5",                     QScriptValue(engine, Qt::Key_5),                  ro);
  widget.setProperty("Key_6",                     QScriptValue(engine, Qt::Key_6),                  ro);
  widget.setProperty("Key_7",                     QScriptValue(engine, Qt::Key_7),                  ro);
  widget.setProperty("Key_8",                     QScriptValue(engine, Qt::Key_8),                  ro);
  widget.setProperty("Key_9",                     QScriptValue(engine, Qt::Key_9),                  ro);
  widget.setProperty("Key_Colon",                 QScriptValue(engine, Qt::Key_Colon),              ro);
  widget.setProperty("Key_Semicolon",             QScriptValue(engine, Qt::Key_Semicolon),          ro);
  widget.setProperty("Key_Less",                  QScriptValue(engine, Qt::Key_Less),               ro);
  widget.setProperty("Key_Equal",                 QScriptValue(engine, Qt::Key_Equal),              ro);
  widget.setProperty("Key_Greater",               QScriptValue(engine, Qt::Key_Greater),            ro);
  widget.setProperty("Key_Question",              QScriptValue(engine, Qt::Key_Question),           ro);
  widget.setProperty("Key_At",                    QScriptValue(engine, Qt::Key_At),                 ro);
  widget.setProperty("Key_A",                     QScriptValue(engine, Qt::Key_A),                  ro);
  widget.setProperty("Key_B",                     QScriptValue(engine, Qt::Key_B),                  ro);
  widget.setProperty("Key_C",                     QScriptValue(engine, Qt::Key_C),                  ro);
  widget.setProperty("Key_D",                     QScriptValue(engine, Qt::Key_D),                  ro);
  widget.setProperty("Key_E",                     QScriptValue(engine, Qt::Key_E),                  ro);
  widget.setProperty("Key_F",                     QScriptValue(engine, Qt::Key_F),                  ro);
  widget.setProperty("Key_G",                     QScriptValue(engine, Qt::Key_G),                  ro);
  widget.setProperty("Key_H",                     QScriptValue(engine, Qt::Key_H),                  ro);
  widget.setProperty("Key_I",                     QScriptValue(engine, Qt::Key_I),                  ro);
  widget.setProperty("Key_J",                     QScriptValue(engine, Qt::Key_J),                  ro);
  widget.setProperty("Key_K",                     QScriptValue(engine, Qt::Key_K),                  ro);
  widget.setProperty("Key_L",                     QScriptValue(engine, Qt::Key_L),                  ro);
  widget.setProperty("Key_M",                     QScriptValue(engine, Qt::Key_M),                  ro);
  widget.setProperty("Key_N",                     QScriptValue(engine, Qt::Key_N),                  ro);
  widget.setProperty("Key_O",                     QScriptValue(engine, Qt::Key_O),                  ro);
  widget.setProperty("Key_P",                     QScriptValue(engine, Qt::Key_P),                  ro);
  widget.setProperty("Key_Q",                     QScriptValue(engine, Qt::Key_Q),                  ro);
  widget.setProperty("Key_R",                     QScriptValue(engine, Qt::Key_R),                  ro);
  widget.setProperty("Key_S",                     QScriptValue(engine, Qt::Key_S),                  ro);
  widget.setProperty("Key_T",                     QScriptValue(engine, Qt::Key_T),                  ro);
  widget.setProperty("Key_U",                     QScriptValue(engine, Qt::Key_U),                  ro);
  widget.setProperty("Key_V",                     QScriptValue(engine, Qt::Key_V),                  ro);
  widget.setProperty("Key_W",                     QScriptValue(engine, Qt::Key_W),                  ro);
  widget.setProperty("Key_X",                     QScriptValue(engine, Qt::Key_X),                  ro);
  widget.setProperty("Key_Y",                     QScriptValue(engine, Qt::Key_Y),                  ro);
  widget.setProperty("Key_Z",                     QScriptValue(engine, Qt::Key_Z),                  ro);
  widget.setProperty("Key_BracketLeft",           QScriptValue(engine, Qt::Key_BracketLeft),        ro);
  widget.setProperty("Key_Backslash",             QScriptValue(engine, Qt::Key_Backslash),          ro);
  widget.setProperty("Key_BracketRight",          QScriptValue(engine, Qt::Key_BracketRight),       ro);
  widget.setProperty("Key_AsciiCircum",           QScriptValue(engine, Qt::Key_AsciiCircum),        ro);
  widget.setProperty("Key_Underscore",            QScriptValue(engine, Qt::Key_Underscore),         ro);
  widget.setProperty("Key_QuoteLeft",             QScriptValue(engine, Qt::Key_QuoteLeft),          ro);
  widget.setProperty("Key_BraceLeft",             QScriptValue(engine, Qt::Key_BraceLeft),          ro);
  widget.setProperty("Key_Bar",                   QScriptValue(engine, Qt::Key_Bar),                ro);
  widget.setProperty("Key_BraceRight",            QScriptValue(engine, Qt::Key_BraceRight),         ro);
  widget.setProperty("Key_AsciiTilde",            QScriptValue(engine, Qt::Key_AsciiTilde),         ro);
  widget.setProperty("Key_nobreakspace",          QScriptValue(engine, Qt::Key_nobreakspace),       ro);
  widget.setProperty("Key_exclamdown",            QScriptValue(engine, Qt::Key_exclamdown),         ro);
  widget.setProperty("Key_cent",                  QScriptValue(engine, Qt::Key_cent),               ro);
  widget.setProperty("Key_sterling",              QScriptValue(engine, Qt::Key_sterling),           ro);
  widget.setProperty("Key_currency",              QScriptValue(engine, Qt::Key_currency),           ro);
  widget.setProperty("Key_yen",                   QScriptValue(engine, Qt::Key_yen),                ro);
  widget.setProperty("Key_brokenbar",             QScriptValue(engine, Qt::Key_brokenbar),          ro);
  widget.setProperty("Key_section",               QScriptValue(engine, Qt::Key_section),            ro);
  widget.setProperty("Key_diaeresis",             QScriptValue(engine, Qt::Key_diaeresis),          ro);
  widget.setProperty("Key_copyright",             QScriptValue(engine, Qt::Key_copyright),          ro);
  widget.setProperty("Key_ordfeminine",           QScriptValue(engine, Qt::Key_ordfeminine),        ro);
  widget.setProperty("Key_guillemotleft",         QScriptValue(engine, Qt::Key_guillemotleft),      ro);
  widget.setProperty("Key_notsign",               QScriptValue(engine, Qt::Key_notsign),            ro);
  widget.setProperty("Key_hyphen",                QScriptValue(engine, Qt::Key_hyphen),             ro);
  widget.setProperty("Key_registered",            QScriptValue(engine, Qt::Key_registered),         ro);
  widget.setProperty("Key_macron",                QScriptValue(engine, Qt::Key_macron),             ro);
  widget.setProperty("Key_degree",                QScriptValue(engine, Qt::Key_degree),             ro);
  widget.setProperty("Key_plusminus",             QScriptValue(engine, Qt::Key_plusminus),          ro);
  widget.setProperty("Key_twosuperior",           QScriptValue(engine, Qt::Key_twosuperior),        ro);
  widget.setProperty("Key_threesuperior",         QScriptValue(engine, Qt::Key_threesuperior),      ro);
  widget.setProperty("Key_acute",                 QScriptValue(engine, Qt::Key_acute),              ro);
  widget.setProperty("Key_mu",                    QScriptValue(engine, Qt::Key_mu),                 ro);
  widget.setProperty("Key_paragraph",             QScriptValue(engine, Qt::Key_paragraph),          ro);
  widget.setProperty("Key_periodcentered",        QScriptValue(engine, Qt::Key_periodcentered),     ro);
  widget.setProperty("Key_cedilla",               QScriptValue(engine, Qt::Key_cedilla),            ro);
  widget.setProperty("Key_onesuperior",           QScriptValue(engine, Qt::Key_onesuperior),        ro);
  widget.setProperty("Key_masculine",             QScriptValue(engine, Qt::Key_masculine),          ro);
  widget.setProperty("Key_guillemotright",        QScriptValue(engine, Qt::Key_guillemotright),     ro);
  widget.setProperty("Key_onequarter",            QScriptValue(engine, Qt::Key_onequarter),         ro);
  widget.setProperty("Key_onehalf",               QScriptValue(engine, Qt::Key_onehalf),            ro);
  widget.setProperty("Key_threequarters",         QScriptValue(engine, Qt::Key_threequarters),      ro);
  widget.setProperty("Key_questiondown",          QScriptValue(engine, Qt::Key_questiondown),       ro);
  widget.setProperty("Key_Agrave",                QScriptValue(engine, Qt::Key_Agrave),             ro);
  widget.setProperty("Key_Aacute",                QScriptValue(engine, Qt::Key_Aacute),             ro);
  widget.setProperty("Key_Acircumflex",           QScriptValue(engine, Qt::Key_Acircumflex),        ro);
  widget.setProperty("Key_Atilde",                QScriptValue(engine, Qt::Key_Atilde),             ro);
  widget.setProperty("Key_Adiaeresis",            QScriptValue(engine, Qt::Key_Adiaeresis),         ro);
  widget.setProperty("Key_Aring",                 QScriptValue(engine, Qt::Key_Aring),              ro);
  widget.setProperty("Key_AE",                    QScriptValue(engine, Qt::Key_AE),                 ro);
  widget.setProperty("Key_Ccedilla",              QScriptValue(engine, Qt::Key_Ccedilla),           ro);
  widget.setProperty("Key_Egrave",                QScriptValue(engine, Qt::Key_Egrave),             ro);
  widget.setProperty("Key_Eacute",                QScriptValue(engine, Qt::Key_Eacute),             ro);
  widget.setProperty("Key_Ecircumflex",           QScriptValue(engine, Qt::Key_Ecircumflex),        ro);
  widget.setProperty("Key_Ediaeresis",            QScriptValue(engine, Qt::Key_Ediaeresis),         ro);
  widget.setProperty("Key_Igrave",                QScriptValue(engine, Qt::Key_Igrave),             ro);
  widget.setProperty("Key_Iacute",                QScriptValue(engine, Qt::Key_Iacute),             ro);
  widget.setProperty("Key_Icircumflex",           QScriptValue(engine, Qt::Key_Icircumflex),        ro);
  widget.setProperty("Key_Idiaeresis",            QScriptValue(engine, Qt::Key_Idiaeresis),         ro);
  widget.setProperty("Key_ETH",                   QScriptValue(engine, Qt::Key_ETH),                ro);
  widget.setProperty("Key_Ntilde",                QScriptValue(engine, Qt::Key_Ntilde),             ro);
  widget.setProperty("Key_Ograve",                QScriptValue(engine, Qt::Key_Ograve),             ro);
  widget.setProperty("Key_Oacute",                QScriptValue(engine, Qt::Key_Oacute),             ro);
  widget.setProperty("Key_Ocircumflex",           QScriptValue(engine, Qt::Key_Ocircumflex),        ro);
  widget.setProperty("Key_Otilde",                QScriptValue(engine, Qt::Key_Otilde),             ro);
  widget.setProperty("Key_Odiaeresis",            QScriptValue(engine, Qt::Key_Odiaeresis),         ro);
  widget.setProperty("Key_multiply",              QScriptValue(engine, Qt::Key_multiply),           ro);
  widget.setProperty("Key_Ooblique",              QScriptValue(engine, Qt::Key_Ooblique),           ro);
  widget.setProperty("Key_Ugrave",                QScriptValue(engine, Qt::Key_Ugrave),             ro);
  widget.setProperty("Key_Uacute",                QScriptValue(engine, Qt::Key_Uacute),             ro);
  widget.setProperty("Key_Ucircumflex",           QScriptValue(engine, Qt::Key_Ucircumflex),        ro);
  widget.setProperty("Key_Udiaeresis",            QScriptValue(engine, Qt::Key_Udiaeresis),         ro);
  widget.setProperty("Key_Yacute",                QScriptValue(engine, Qt::Key_Yacute),             ro);
  widget.setProperty("Key_THORN",                 QScriptValue(engine, Qt::Key_THORN),              ro);
  widget.setProperty("Key_ssharp",                QScriptValue(engine, Qt::Key_ssharp),             ro);
  widget.setProperty("Key_division",              QScriptValue(engine, Qt::Key_division),           ro);
  widget.setProperty("Key_ydiaeresis",            QScriptValue(engine, Qt::Key_ydiaeresis),         ro);
  widget.setProperty("Key_Multi_key",             QScriptValue(engine, Qt::Key_Multi_key),          ro);
  widget.setProperty("Key_Codeinput",             QScriptValue(engine, Qt::Key_Codeinput),          ro);
  widget.setProperty("Key_SingleCandidate",       QScriptValue(engine, Qt::Key_SingleCandidate),    ro);
  widget.setProperty("Key_MultipleCandidate",     QScriptValue(engine, Qt::Key_MultipleCandidate),  ro);
  widget.setProperty("Key_PreviousCandidate",     QScriptValue(engine, Qt::Key_PreviousCandidate),  ro);
  widget.setProperty("Key_Mode_switch",           QScriptValue(engine, Qt::Key_Mode_switch),        ro);
  widget.setProperty("Key_Kanji",                 QScriptValue(engine, Qt::Key_Kanji),              ro);
  widget.setProperty("Key_Muhenkan",              QScriptValue(engine, Qt::Key_Muhenkan),           ro);
  widget.setProperty("Key_Henkan",                QScriptValue(engine, Qt::Key_Henkan),             ro);
  widget.setProperty("Key_Romaji",                QScriptValue(engine, Qt::Key_Romaji),             ro);
  widget.setProperty("Key_Hiragana",              QScriptValue(engine, Qt::Key_Hiragana),           ro);
  widget.setProperty("Key_Katakana",              QScriptValue(engine, Qt::Key_Katakana),           ro);
  widget.setProperty("Key_Hiragana_Katakana",     QScriptValue(engine, Qt::Key_Hiragana_Katakana),  ro);
  widget.setProperty("Key_Zenkaku",               QScriptValue(engine, Qt::Key_Zenkaku),            ro);
  widget.setProperty("Key_Hankaku",               QScriptValue(engine, Qt::Key_Hankaku),            ro);
  widget.setProperty("Key_Zenkaku_Hankaku",       QScriptValue(engine, Qt::Key_Zenkaku_Hankaku),    ro);
  widget.setProperty("Key_Touroku",               QScriptValue(engine, Qt::Key_Touroku),            ro);
  widget.setProperty("Key_Massyo",                QScriptValue(engine, Qt::Key_Massyo),             ro);
  widget.setProperty("Key_Kana_Lock",             QScriptValue(engine, Qt::Key_Kana_Lock),          ro);
  widget.setProperty("Key_Kana_Shift",            QScriptValue(engine, Qt::Key_Kana_Shift),         ro);
  widget.setProperty("Key_Eisu_Shift",            QScriptValue(engine, Qt::Key_Eisu_Shift),         ro);
  widget.setProperty("Key_Eisu_toggle",           QScriptValue(engine, Qt::Key_Eisu_toggle),        ro);
  widget.setProperty("Key_Hangul",                QScriptValue(engine, Qt::Key_Hangul),             ro);
  widget.setProperty("Key_Hangul_Start",          QScriptValue(engine, Qt::Key_Hangul_Start),       ro);
  widget.setProperty("Key_Hangul_End",            QScriptValue(engine, Qt::Key_Hangul_End),         ro);
  widget.setProperty("Key_Hangul_Hanja",          QScriptValue(engine, Qt::Key_Hangul_Hanja),       ro);
  widget.setProperty("Key_Hangul_Jamo",           QScriptValue(engine, Qt::Key_Hangul_Jamo),        ro);
  widget.setProperty("Key_Hangul_Romaja",         QScriptValue(engine, Qt::Key_Hangul_Romaja),      ro);
  widget.setProperty("Key_Hangul_Jeonja",         QScriptValue(engine, Qt::Key_Hangul_Jeonja),      ro);
  widget.setProperty("Key_Hangul_Banja",          QScriptValue(engine, Qt::Key_Hangul_Banja),       ro);
  widget.setProperty("Key_Hangul_PreHanja",       QScriptValue(engine, Qt::Key_Hangul_PreHanja),    ro);
  widget.setProperty("Key_Hangul_PostHanja",      QScriptValue(engine, Qt::Key_Hangul_PostHanja),   ro);
  widget.setProperty("Key_Hangul_Special",        QScriptValue(engine, Qt::Key_Hangul_Special),     ro);
  widget.setProperty("Key_Dead_Grave",            QScriptValue(engine, Qt::Key_Dead_Grave),         ro);
  widget.setProperty("Key_Dead_Acute",            QScriptValue(engine, Qt::Key_Dead_Acute),         ro);
  widget.setProperty("Key_Dead_Circumflex",       QScriptValue(engine, Qt::Key_Dead_Circumflex),    ro);
  widget.setProperty("Key_Dead_Tilde",            QScriptValue(engine, Qt::Key_Dead_Tilde),         ro);
  widget.setProperty("Key_Dead_Macron",           QScriptValue(engine, Qt::Key_Dead_Macron),        ro);
  widget.setProperty("Key_Dead_Breve",            QScriptValue(engine, Qt::Key_Dead_Breve),         ro);
  widget.setProperty("Key_Dead_Abovedot",         QScriptValue(engine, Qt::Key_Dead_Abovedot),      ro);
  widget.setProperty("Key_Dead_Diaeresis",        QScriptValue(engine, Qt::Key_Dead_Diaeresis),     ro);
  widget.setProperty("Key_Dead_Abovering",        QScriptValue(engine, Qt::Key_Dead_Abovering),     ro);
  widget.setProperty("Key_Dead_Doubleacute",      QScriptValue(engine, Qt::Key_Dead_Doubleacute),   ro);
  widget.setProperty("Key_Dead_Caron",            QScriptValue(engine, Qt::Key_Dead_Caron),         ro);
  widget.setProperty("Key_Dead_Cedilla",          QScriptValue(engine, Qt::Key_Dead_Cedilla),       ro);
  widget.setProperty("Key_Dead_Ogonek",           QScriptValue(engine, Qt::Key_Dead_Ogonek),        ro);
  widget.setProperty("Key_Dead_Iota",             QScriptValue(engine, Qt::Key_Dead_Iota),          ro);
  widget.setProperty("Key_Dead_Voiced_Sound",     QScriptValue(engine, Qt::Key_Dead_Voiced_Sound),  ro);
  widget.setProperty("Key_Dead_Semivoiced_Sound", QScriptValue(engine, Qt::Key_Dead_Semivoiced_Sound), ro);
  widget.setProperty("Key_Dead_Belowdot",         QScriptValue(engine, Qt::Key_Dead_Belowdot),      ro);
  widget.setProperty("Key_Dead_Hook",             QScriptValue(engine, Qt::Key_Dead_Hook),          ro);
  widget.setProperty("Key_Dead_Horn",             QScriptValue(engine, Qt::Key_Dead_Horn),          ro);
  widget.setProperty("Key_Back",                  QScriptValue(engine, Qt::Key_Back),               ro);
  widget.setProperty("Key_Forward",               QScriptValue(engine, Qt::Key_Forward),            ro);
  widget.setProperty("Key_Stop",                  QScriptValue(engine, Qt::Key_Stop),               ro);
  widget.setProperty("Key_Refresh",               QScriptValue(engine, Qt::Key_Refresh),            ro);
  widget.setProperty("Key_VolumeDown",            QScriptValue(engine, Qt::Key_VolumeDown),         ro);
  widget.setProperty("Key_VolumeMute",            QScriptValue(engine, Qt::Key_VolumeMute),         ro);
  widget.setProperty("Key_VolumeUp",              QScriptValue(engine, Qt::Key_VolumeUp),           ro);
  widget.setProperty("Key_BassBoost",             QScriptValue(engine, Qt::Key_BassBoost),          ro);
  widget.setProperty("Key_BassUp",                QScriptValue(engine, Qt::Key_BassUp),             ro);
  widget.setProperty("Key_BassDown",              QScriptValue(engine, Qt::Key_BassDown),           ro);
  widget.setProperty("Key_TrebleUp",              QScriptValue(engine, Qt::Key_TrebleUp),           ro);
  widget.setProperty("Key_TrebleDown",            QScriptValue(engine, Qt::Key_TrebleDown),         ro);
  widget.setProperty("Key_MediaPlay",             QScriptValue(engine, Qt::Key_MediaPlay),          ro);
  widget.setProperty("Key_MediaStop",             QScriptValue(engine, Qt::Key_MediaStop),          ro);
  widget.setProperty("Key_MediaPrevious",         QScriptValue(engine, Qt::Key_MediaPrevious),      ro);
  widget.setProperty("Key_MediaNext",             QScriptValue(engine, Qt::Key_MediaNext),          ro);
  widget.setProperty("Key_MediaRecord",           QScriptValue(engine, Qt::Key_MediaRecord),        ro);
  widget.setProperty("Key_MediaPause",            QScriptValue(engine, Qt::Key_MediaPause),         ro);
  widget.setProperty("Key_MediaTogglePlayPause",  QScriptValue(engine, Qt::Key_MediaTogglePlayPause), ro);
  widget.setProperty("Key_HomePage",              QScriptValue(engine, Qt::Key_HomePage),           ro);
  widget.setProperty("Key_Favorites",             QScriptValue(engine, Qt::Key_Favorites),          ro);
  widget.setProperty("Key_Search",                QScriptValue(engine, Qt::Key_Search),             ro);
  widget.setProperty("Key_Standby",               QScriptValue(engine, Qt::Key_Standby),            ro);
  widget.setProperty("Key_OpenUrl",               QScriptValue(engine, Qt::Key_OpenUrl),            ro);
  widget.setProperty("Key_LaunchMail",            QScriptValue(engine, Qt::Key_LaunchMail),         ro);
  widget.setProperty("Key_LaunchMedia",           QScriptValue(engine, Qt::Key_LaunchMedia),        ro);
  widget.setProperty("Key_Launch0",               QScriptValue(engine, Qt::Key_Launch0),            ro);
  widget.setProperty("Key_Launch1",               QScriptValue(engine, Qt::Key_Launch1),            ro);
  widget.setProperty("Key_Launch2",               QScriptValue(engine, Qt::Key_Launch2),            ro);
  widget.setProperty("Key_Launch3",               QScriptValue(engine, Qt::Key_Launch3),            ro);
  widget.setProperty("Key_Launch4",               QScriptValue(engine, Qt::Key_Launch4),            ro);
  widget.setProperty("Key_Launch5",               QScriptValue(engine, Qt::Key_Launch5),            ro);
  widget.setProperty("Key_Launch6",               QScriptValue(engine, Qt::Key_Launch6),            ro);
  widget.setProperty("Key_Launch7",               QScriptValue(engine, Qt::Key_Launch7),            ro);
  widget.setProperty("Key_Launch8",               QScriptValue(engine, Qt::Key_Launch8),            ro);
  widget.setProperty("Key_Launch9",               QScriptValue(engine, Qt::Key_Launch9),            ro);
  widget.setProperty("Key_LaunchA",               QScriptValue(engine, Qt::Key_LaunchA),            ro);
  widget.setProperty("Key_LaunchB",               QScriptValue(engine, Qt::Key_LaunchB),            ro);
  widget.setProperty("Key_LaunchC",               QScriptValue(engine, Qt::Key_LaunchC),            ro);
  widget.setProperty("Key_LaunchD",               QScriptValue(engine, Qt::Key_LaunchD),            ro);
  widget.setProperty("Key_LaunchE",               QScriptValue(engine, Qt::Key_LaunchE),            ro);
  widget.setProperty("Key_LaunchF",               QScriptValue(engine, Qt::Key_LaunchF),            ro);
  widget.setProperty("Key_LaunchG",               QScriptValue(engine, Qt::Key_LaunchG),            ro);
  widget.setProperty("Key_LaunchH",               QScriptValue(engine, Qt::Key_LaunchH),            ro);
  widget.setProperty("Key_MonBrightnessUp",        QScriptValue(engine, Qt::Key_MonBrightnessUp),        ro);
  widget.setProperty("Key_MonBrightnessDown",      QScriptValue(engine, Qt::Key_MonBrightnessDown),      ro);
  widget.setProperty("Key_KeyboardLightOnOff",     QScriptValue(engine, Qt::Key_KeyboardLightOnOff),     ro);
  widget.setProperty("Key_KeyboardBrightnessUp",   QScriptValue(engine, Qt::Key_KeyboardBrightnessUp),   ro);
  widget.setProperty("Key_KeyboardBrightnessDown", QScriptValue(engine, Qt::Key_KeyboardBrightnessDown), ro);
  widget.setProperty("Key_PowerOff",               QScriptValue(engine, Qt::Key_PowerOff),               ro);
  widget.setProperty("Key_WakeUp",                 QScriptValue(engine, Qt::Key_WakeUp),                 ro);
  widget.setProperty("Key_Eject",                  QScriptValue(engine, Qt::Key_Eject),                  ro);
  widget.setProperty("Key_ScreenSaver",            QScriptValue(engine, Qt::Key_ScreenSaver),            ro);
  widget.setProperty("Key_WWW",                    QScriptValue(engine, Qt::Key_WWW),                    ro);
  widget.setProperty("Key_Memo",                   QScriptValue(engine, Qt::Key_Memo),                   ro);
  widget.setProperty("Key_LightBulb",              QScriptValue(engine, Qt::Key_LightBulb),              ro);
  widget.setProperty("Key_Shop",                   QScriptValue(engine, Qt::Key_Shop),                   ro);
  widget.setProperty("Key_History",                QScriptValue(engine, Qt::Key_History),                ro);
  widget.setProperty("Key_AddFavorite",            QScriptValue(engine, Qt::Key_AddFavorite),            ro);
  widget.setProperty("Key_HotLinks",               QScriptValue(engine, Qt::Key_HotLinks),               ro);
  widget.setProperty("Key_BrightnessAdjust",       QScriptValue(engine, Qt::Key_BrightnessAdjust),       ro);
  widget.setProperty("Key_Finance",                QScriptValue(engine, Qt::Key_Finance),                ro);
  widget.setProperty("Key_Community",              QScriptValue(engine, Qt::Key_Community),              ro);
  widget.setProperty("Key_AudioRewind",            QScriptValue(engine, Qt::Key_AudioRewind),            ro);
  widget.setProperty("Key_BackForward",            QScriptValue(engine, Qt::Key_BackForward),            ro);
  widget.setProperty("Key_ApplicationLeft",        QScriptValue(engine, Qt::Key_ApplicationLeft),        ro);
  widget.setProperty("Key_ApplicationRight",       QScriptValue(engine, Qt::Key_ApplicationRight),       ro);
  widget.setProperty("Key_Book",                   QScriptValue(engine, Qt::Key_Book),                   ro);
  widget.setProperty("Key_CD",                     QScriptValue(engine, Qt::Key_CD),                     ro);
  widget.setProperty("Key_Calculator",             QScriptValue(engine, Qt::Key_Calculator),             ro);
  widget.setProperty("Key_ToDoList",               QScriptValue(engine, Qt::Key_ToDoList),               ro);
  widget.setProperty("Key_ClearGrab",              QScriptValue(engine, Qt::Key_ClearGrab),              ro);
  widget.setProperty("Key_Close",                  QScriptValue(engine, Qt::Key_Close),                  ro);
  widget.setProperty("Key_Copy",                   QScriptValue(engine, Qt::Key_Copy),                   ro);
  widget.setProperty("Key_Cut",                    QScriptValue(engine, Qt::Key_Cut),                    ro);
  widget.setProperty("Key_Display",                QScriptValue(engine, Qt::Key_Display),                ro);
  widget.setProperty("Key_DOS",                    QScriptValue(engine, Qt::Key_DOS),                    ro);
  widget.setProperty("Key_Documents",              QScriptValue(engine, Qt::Key_Documents),              ro);
  widget.setProperty("Key_Excel",                  QScriptValue(engine, Qt::Key_Excel),                  ro);
  widget.setProperty("Key_Explorer",               QScriptValue(engine, Qt::Key_Explorer),               ro);
  widget.setProperty("Key_Game",                   QScriptValue(engine, Qt::Key_Game),                   ro);
  widget.setProperty("Key_Go",                     QScriptValue(engine, Qt::Key_Go),                     ro);
  widget.setProperty("Key_iTouch",                 QScriptValue(engine, Qt::Key_iTouch),                 ro);
  widget.setProperty("Key_LogOff",                 QScriptValue(engine, Qt::Key_LogOff),                 ro);
  widget.setProperty("Key_Market",                 QScriptValue(engine, Qt::Key_Market),                 ro);
  widget.setProperty("Key_Meeting",                QScriptValue(engine, Qt::Key_Meeting),                ro);
  widget.setProperty("Key_MenuKB",                 QScriptValue(engine, Qt::Key_MenuKB),                 ro);
  widget.setProperty("Key_MenuPB",                 QScriptValue(engine, Qt::Key_MenuPB),                 ro);
  widget.setProperty("Key_MySites",                QScriptValue(engine, Qt::Key_MySites),                ro);
  widget.setProperty("Key_News",                   QScriptValue(engine, Qt::Key_News),                   ro);
  widget.setProperty("Key_OfficeHome",             QScriptValue(engine, Qt::Key_OfficeHome),             ro);
  widget.setProperty("Key_Option",                 QScriptValue(engine, Qt::Key_Option),                 ro);
  widget.setProperty("Key_Paste",                  QScriptValue(engine, Qt::Key_Paste),                  ro);
  widget.setProperty("Key_Phone",                  QScriptValue(engine, Qt::Key_Phone),                  ro);
  widget.setProperty("Key_Calendar",               QScriptValue(engine, Qt::Key_Calendar),               ro);
  widget.setProperty("Key_Reply",                  QScriptValue(engine, Qt::Key_Reply),                  ro);
  widget.setProperty("Key_Reload",                 QScriptValue(engine, Qt::Key_Reload),                 ro);
  widget.setProperty("Key_RotateWindows",          QScriptValue(engine, Qt::Key_RotateWindows),          ro);
  widget.setProperty("Key_RotationPB",             QScriptValue(engine, Qt::Key_RotationPB),             ro);
  widget.setProperty("Key_RotationKB",             QScriptValue(engine, Qt::Key_RotationKB),             ro);
  widget.setProperty("Key_Save",                   QScriptValue(engine, Qt::Key_Save),                   ro);
  widget.setProperty("Key_Send",                   QScriptValue(engine, Qt::Key_Send),                   ro);
  widget.setProperty("Key_Spell",                  QScriptValue(engine, Qt::Key_Spell),                  ro);
  widget.setProperty("Key_SplitScreen",            QScriptValue(engine, Qt::Key_SplitScreen),            ro);
  widget.setProperty("Key_Support",                QScriptValue(engine, Qt::Key_Support),                ro);
  widget.setProperty("Key_TaskPane",               QScriptValue(engine, Qt::Key_TaskPane),               ro);
  widget.setProperty("Key_Terminal",               QScriptValue(engine, Qt::Key_Terminal),               ro);
  widget.setProperty("Key_Tools",                  QScriptValue(engine, Qt::Key_Tools),                  ro);
  widget.setProperty("Key_Travel",                 QScriptValue(engine, Qt::Key_Travel),                 ro);
  widget.setProperty("Key_Video",                  QScriptValue(engine, Qt::Key_Video),                  ro);
  widget.setProperty("Key_Word",                   QScriptValue(engine, Qt::Key_Word),                   ro);
  widget.setProperty("Key_Xfer",                   QScriptValue(engine, Qt::Key_Xfer),                   ro);
  widget.setProperty("Key_ZoomIn",                 QScriptValue(engine, Qt::Key_ZoomIn),                 ro);
  widget.setProperty("Key_ZoomOut",                QScriptValue(engine, Qt::Key_ZoomOut),                ro);
  widget.setProperty("Key_Away",                   QScriptValue(engine, Qt::Key_Away),                   ro);
  widget.setProperty("Key_Messenger",              QScriptValue(engine, Qt::Key_Messenger),              ro);
  widget.setProperty("Key_WebCam",                 QScriptValue(engine, Qt::Key_WebCam),                 ro);
  widget.setProperty("Key_MailForward",            QScriptValue(engine, Qt::Key_MailForward),            ro);
  widget.setProperty("Key_Pictures",               QScriptValue(engine, Qt::Key_Pictures),               ro);
  widget.setProperty("Key_Music",                  QScriptValue(engine, Qt::Key_Music),                  ro);
  widget.setProperty("Key_Battery",                QScriptValue(engine, Qt::Key_Battery),                ro);
  widget.setProperty("Key_Bluetooth",              QScriptValue(engine, Qt::Key_Bluetooth),              ro);
  widget.setProperty("Key_WLAN",                   QScriptValue(engine, Qt::Key_WLAN),                   ro);
  widget.setProperty("Key_UWB",                    QScriptValue(engine, Qt::Key_UWB),                    ro);
  widget.setProperty("Key_AudioForward",           QScriptValue(engine, Qt::Key_AudioForward),           ro);
  widget.setProperty("Key_AudioRepeat",            QScriptValue(engine, Qt::Key_AudioRepeat),            ro);
  widget.setProperty("Key_AudioRandomPlay",        QScriptValue(engine, Qt::Key_AudioRandomPlay),        ro);
  widget.setProperty("Key_Subtitle",               QScriptValue(engine, Qt::Key_Subtitle),               ro);
  widget.setProperty("Key_AudioCycleTrack",        QScriptValue(engine, Qt::Key_AudioCycleTrack),        ro);
  widget.setProperty("Key_Time",                   QScriptValue(engine, Qt::Key_Time),                   ro);
  widget.setProperty("Key_Hibernate",              QScriptValue(engine, Qt::Key_Hibernate),              ro);
  widget.setProperty("Key_View",                   QScriptValue(engine, Qt::Key_View),                   ro);
  widget.setProperty("Key_TopMenu",                QScriptValue(engine, Qt::Key_TopMenu),                ro);
  widget.setProperty("Key_PowerDown",              QScriptValue(engine, Qt::Key_PowerDown),              ro);
  widget.setProperty("Key_Suspend",                QScriptValue(engine, Qt::Key_Suspend),                ro);
  widget.setProperty("Key_ContrastAdjust",         QScriptValue(engine, Qt::Key_ContrastAdjust),         ro);
  widget.setProperty("Key_TouchpadToggle",         QScriptValue(engine, Qt::Key_TouchpadToggle),         ro);
  widget.setProperty("Key_TouchpadOn",             QScriptValue(engine, Qt::Key_TouchpadOn),             ro);
  widget.setProperty("Key_TouchpadOff",            QScriptValue(engine, Qt::Key_TouchpadOff),            ro);
  widget.setProperty("Key_MicMute",                QScriptValue(engine, Qt::Key_MicMute),                ro);
  widget.setProperty("Key_Red",                    QScriptValue(engine, Qt::Key_Red),                    ro);
  widget.setProperty("Key_Green",                  QScriptValue(engine, Qt::Key_Green),                  ro);
  widget.setProperty("Key_Yellow",                 QScriptValue(engine, Qt::Key_Yellow),                 ro);
  widget.setProperty("Key_Blue",                   QScriptValue(engine, Qt::Key_Blue),                   ro);
  widget.setProperty("Key_ChannelUp",              QScriptValue(engine, Qt::Key_ChannelUp),              ro);
  widget.setProperty("Key_ChannelDown",            QScriptValue(engine, Qt::Key_ChannelDown),            ro);
  widget.setProperty("Key_Guide",                  QScriptValue(engine, Qt::Key_Guide),                  ro);
  widget.setProperty("Key_Info",                   QScriptValue(engine, Qt::Key_Info),                   ro);
  widget.setProperty("Key_Settings",               QScriptValue(engine, Qt::Key_Settings),               ro);
  widget.setProperty("Key_MicVolumeUp",            QScriptValue(engine, Qt::Key_MicVolumeUp),            ro);
  widget.setProperty("Key_MicVolumeDown",          QScriptValue(engine, Qt::Key_MicVolumeDown),          ro);
  widget.setProperty("Key_New",                    QScriptValue(engine, Qt::Key_New),                    ro);
  widget.setProperty("Key_Open",                   QScriptValue(engine, Qt::Key_Open),                   ro);
  widget.setProperty("Key_Find",                   QScriptValue(engine, Qt::Key_Find),                   ro);
  widget.setProperty("Key_Undo",                   QScriptValue(engine, Qt::Key_Undo),                   ro);
  widget.setProperty("Key_Redo",                   QScriptValue(engine, Qt::Key_Redo),                   ro);
  widget.setProperty("Key_MediaLast",              QScriptValue(engine, Qt::Key_MediaLast),              ro);
  widget.setProperty("Key_unknown",                QScriptValue(engine, Qt::Key_unknown),                ro);
  widget.setProperty("Key_Call",                   QScriptValue(engine, Qt::Key_Call),                   ro);
  widget.setProperty("Key_Camera",                 QScriptValue(engine, Qt::Key_Camera),                 ro);
  widget.setProperty("Key_CameraFocus",            QScriptValue(engine, Qt::Key_CameraFocus),            ro);
  widget.setProperty("Key_Context1",               QScriptValue(engine, Qt::Key_Context1),               ro);
  widget.setProperty("Key_Context2",               QScriptValue(engine, Qt::Key_Context2),               ro);
  widget.setProperty("Key_Context3",               QScriptValue(engine, Qt::Key_Context3),               ro);
  widget.setProperty("Key_Context4",               QScriptValue(engine, Qt::Key_Context4),               ro);
  widget.setProperty("Key_Flip",                   QScriptValue(engine, Qt::Key_Flip),                   ro);
  widget.setProperty("Key_Hangup",                 QScriptValue(engine, Qt::Key_Hangup),                 ro);
  widget.setProperty("Key_No",                     QScriptValue(engine, Qt::Key_No),                     ro);
  widget.setProperty("Key_Select",                 QScriptValue(engine, Qt::Key_Select),                 ro);
  widget.setProperty("Key_Yes",                    QScriptValue(engine, Qt::Key_Yes),                    ro);
  widget.setProperty("Key_ToggleCallHangup",       QScriptValue(engine, Qt::Key_ToggleCallHangup),       ro);
  widget.setProperty("Key_VoiceDial",              QScriptValue(engine, Qt::Key_VoiceDial),              ro);
  widget.setProperty("Key_LastNumberRedial",       QScriptValue(engine, Qt::Key_LastNumberRedial),       ro);
  widget.setProperty("Key_Execute",                QScriptValue(engine, Qt::Key_Execute),                ro);
  widget.setProperty("Key_Printer",                QScriptValue(engine, Qt::Key_Printer),                ro);
  widget.setProperty("Key_Play",                   QScriptValue(engine, Qt::Key_Play),                   ro);
  widget.setProperty("Key_Sleep",                  QScriptValue(engine, Qt::Key_Sleep),                  ro);
  widget.setProperty("Key_Zoom",                   QScriptValue(engine, Qt::Key_Zoom),                   ro);
  widget.setProperty("Key_Exit",                   QScriptValue(engine, Qt::Key_Exit),                   ro);
  widget.setProperty("Key_Cancel",                 QScriptValue(engine, Qt::Key_Cancel),                 ro);

  qScriptRegisterMetaType(engine, KeyboardModifierToScriptValue, KeyboardModifierFromScriptValue);
  widget.setProperty("NoModifier",                QScriptValue(engine, Qt::NoModifier),             ro);
  widget.setProperty("ShiftModifier",             QScriptValue(engine, Qt::ShiftModifier),          ro);
  widget.setProperty("ControlModifier",           QScriptValue(engine, Qt::ControlModifier),        ro);
  widget.setProperty("AltModifier",               QScriptValue(engine, Qt::AltModifier),            ro);
  widget.setProperty("MetaModifier",              QScriptValue(engine, Qt::MetaModifier),           ro);
  widget.setProperty("KeypadModifier",            QScriptValue(engine, Qt::KeypadModifier),         ro);
  widget.setProperty("GroupSwitchModifier",       QScriptValue(engine, Qt::GroupSwitchModifier),    ro);

  qScriptRegisterMetaType(engine, LayoutDirectionToScriptValue, LayoutDirectionFromScriptValue);
  widget.setProperty("LeftToRight",               QScriptValue(engine, Qt::LeftToRight),            ro);
  widget.setProperty("RightToLeft",               QScriptValue(engine, Qt::RightToLeft),            ro);
  widget.setProperty("LayoutDirectionAuto",       QScriptValue(engine, Qt::LayoutDirectionAuto),    ro);

  qScriptRegisterMetaType(engine, MaskModeToScriptValue, MaskModeFromScriptValue);
  widget.setProperty("MaskInColor",               QScriptValue(engine, Qt::MaskInColor),            ro);
  widget.setProperty("MaskOutColor",              QScriptValue(engine, Qt::MaskOutColor),           ro);

  qScriptRegisterMetaType(engine, MatchFlagToScriptValue,  MatchFlagFromScriptValue);
  qScriptRegisterMetaType(engine, MatchFlagsToScriptValue, MatchFlagsFromScriptValue);
  widget.setProperty("MatchExactly",              QScriptValue(engine, Qt::MatchExactly),           ro);
  widget.setProperty("MatchFixedString",          QScriptValue(engine, Qt::MatchFixedString),       ro);
  widget.setProperty("MatchContains",             QScriptValue(engine, Qt::MatchContains),          ro);
  widget.setProperty("MatchStartsWith",           QScriptValue(engine, Qt::MatchStartsWith),        ro);
  widget.setProperty("MatchEndsWith",             QScriptValue(engine, Qt::MatchEndsWith),          ro);
  widget.setProperty("MatchCaseSensitive",        QScriptValue(engine, Qt::MatchCaseSensitive),     ro);
  widget.setProperty("MatchRegExp",               QScriptValue(engine, Qt::MatchRegExp),            ro);
  widget.setProperty("MatchWildcard",             QScriptValue(engine, Qt::MatchWildcard),          ro);
  widget.setProperty("MatchWrap",                 QScriptValue(engine, Qt::MatchWrap),              ro);
  widget.setProperty("MatchRecursive",            QScriptValue(engine, Qt::MatchRecursive),         ro);

  qScriptRegisterMetaType(engine, ModifierToScriptValue, ModifierFromScriptValue);
  widget.setProperty("SHIFT",                     QScriptValue(engine, Qt::SHIFT),                  ro);
  widget.setProperty("META",                      QScriptValue(engine, Qt::META),                   ro);
  widget.setProperty("CTRL",                      QScriptValue(engine, Qt::CTRL),                   ro);
  widget.setProperty("ALT",                       QScriptValue(engine, Qt::ALT),                    ro);
  widget.setProperty("UNICODE_ACCEL",             QScriptValue(engine, Qt::UNICODE_ACCEL),          ro);

  qScriptRegisterMetaType(engine, MouseButtonToScriptValue,  MouseButtonFromScriptValue);
  qScriptRegisterMetaType(engine, MouseButtonsToScriptValue, MouseButtonsFromScriptValue);
  widget.setProperty("NoButton",                  QScriptValue(engine, Qt::NoButton),               ro);
  widget.setProperty("AllButtons",                QScriptValue(engine, Qt::AllButtons),             ro);
  widget.setProperty("LeftButton",                QScriptValue(engine, Qt::LeftButton),             ro);
  widget.setProperty("RightButton",               QScriptValue(engine, Qt::RightButton),            ro);
  widget.setProperty("MidButton",                 QScriptValue(engine, Qt::MidButton),              ro);
  widget.setProperty("MiddleButton",              QScriptValue(engine, Qt::MiddleButton),           ro);
  widget.setProperty("BackButton",                QScriptValue(engine, Qt::BackButton),             ro);
  widget.setProperty("XButton1",                  QScriptValue(engine, Qt::XButton1),               ro);
  widget.setProperty("ExtraButton1",              QScriptValue(engine, Qt::ExtraButton1),           ro);
  widget.setProperty("ForwardButton",             QScriptValue(engine, Qt::ForwardButton),          ro);
  widget.setProperty("XButton2",                  QScriptValue(engine, Qt::XButton2),               ro);
  widget.setProperty("ExtraButton2",              QScriptValue(engine, Qt::ExtraButton2),           ro);
  widget.setProperty("TaskButton",                QScriptValue(engine, Qt::TaskButton),             ro);
  widget.setProperty("ExtraButton3",              QScriptValue(engine, Qt::ExtraButton3),           ro);
  widget.setProperty("ExtraButton4",              QScriptValue(engine, Qt::ExtraButton4),           ro);
  widget.setProperty("ExtraButton5",              QScriptValue(engine, Qt::ExtraButton5),           ro);
  widget.setProperty("ExtraButton6",              QScriptValue(engine, Qt::ExtraButton6),           ro);
  widget.setProperty("ExtraButton7",              QScriptValue(engine, Qt::ExtraButton7),           ro);
  widget.setProperty("ExtraButton8",              QScriptValue(engine, Qt::ExtraButton8),           ro);
  widget.setProperty("ExtraButton9",              QScriptValue(engine, Qt::ExtraButton9),           ro);
  widget.setProperty("ExtraButton10",             QScriptValue(engine, Qt::ExtraButton10),          ro);
  widget.setProperty("ExtraButton11",             QScriptValue(engine, Qt::ExtraButton11),          ro);
  widget.setProperty("ExtraButton12",             QScriptValue(engine, Qt::ExtraButton12),          ro);
  widget.setProperty("ExtraButton13",             QScriptValue(engine, Qt::ExtraButton13),          ro);
  widget.setProperty("ExtraButton14",             QScriptValue(engine, Qt::ExtraButton14),          ro);
  widget.setProperty("ExtraButton15",             QScriptValue(engine, Qt::ExtraButton15),          ro);
  widget.setProperty("ExtraButton16",             QScriptValue(engine, Qt::ExtraButton16),          ro);
  widget.setProperty("ExtraButton17",             QScriptValue(engine, Qt::ExtraButton17),          ro);
  widget.setProperty("ExtraButton18",             QScriptValue(engine, Qt::ExtraButton18),          ro);
  widget.setProperty("ExtraButton19",             QScriptValue(engine, Qt::ExtraButton19),          ro);
  widget.setProperty("ExtraButton20",             QScriptValue(engine, Qt::ExtraButton20),          ro);
  widget.setProperty("ExtraButton21",             QScriptValue(engine, Qt::ExtraButton21),          ro);
  widget.setProperty("ExtraButton22",             QScriptValue(engine, Qt::ExtraButton22),          ro);
  widget.setProperty("ExtraButton23",             QScriptValue(engine, Qt::ExtraButton23),          ro);
  widget.setProperty("ExtraButton24",             QScriptValue(engine, Qt::ExtraButton24),          ro);
  widget.setProperty("ExtraButton20",             QScriptValue(engine, Qt::ExtraButton20),          ro);

  qScriptRegisterMetaType(engine, MouseEventFlagToScriptValue,  MouseEventFlagFromScriptValue);
  qScriptRegisterMetaType(engine, MouseEventFlagsToScriptValue, MouseEventFlagsFromScriptValue);
  widget.setProperty("MouseEventCreatedDoubleClick", QScriptValue(engine, Qt::MouseEventCreatedDoubleClick), ro);

  qScriptRegisterMetaType(engine, MouseEventSourceToScriptValue,  MouseEventSourceFromScriptValue);
  widget.setProperty("MouseEventNotSynthesized",           QScriptValue(engine, Qt::MouseEventNotSynthesized),           ro);
  widget.setProperty("MouseEventSynthesizedBySystem",      QScriptValue(engine, Qt::MouseEventSynthesizedBySystem),      ro);
  widget.setProperty("MouseEventSynthesizedByQt",          QScriptValue(engine, Qt::MouseEventSynthesizedByQt),          ro);
#if QT_VERSION >= 0x050600
  widget.setProperty("MouseEventSynthesizedByApplication", QScriptValue(engine, Qt::MouseEventSynthesizedByApplication), ro);
#endif

  qScriptRegisterMetaType(engine, NativeGestureTypeToScriptValue,  NativeGestureTypeFromScriptValue);
  widget.setProperty("BeginNativeGesture",      QScriptValue(engine, Qt::BeginNativeGesture),     ro);
  widget.setProperty("EndNativeGesture",        QScriptValue(engine, Qt::EndNativeGesture),       ro);
  widget.setProperty("PanNativeGesture",        QScriptValue(engine, Qt::PanNativeGesture),       ro);
  widget.setProperty("ZoomNativeGesture",       QScriptValue(engine, Qt::ZoomNativeGesture),      ro);
  widget.setProperty("SmartZoomNativeGesture",  QScriptValue(engine, Qt::SmartZoomNativeGesture), ro);
  widget.setProperty("RotateNativeGesture",     QScriptValue(engine, Qt::RotateNativeGesture),    ro);
  widget.setProperty("SwipeNativeGesture",      QScriptValue(engine, Qt::SwipeNativeGesture),     ro);

  qScriptRegisterMetaType(engine, NavigationModeToScriptValue,  NavigationModeFromScriptValue);
  widget.setProperty("NavigationModeNone",                QScriptValue(engine, Qt::NavigationModeNone),               ro);
  widget.setProperty("NavigationModeKeypadTabOrder",      QScriptValue(engine, Qt::NavigationModeKeypadTabOrder),     ro);
  widget.setProperty("NavigationModeKeypadDirectional",   QScriptValue(engine, Qt::NavigationModeKeypadDirectional),  ro);
  widget.setProperty("NavigationModeCursorAuto",          QScriptValue(engine, Qt::NavigationModeCursorAuto),         ro);
  widget.setProperty("NavigationModeCursorForceVisible",  QScriptValue(engine, Qt::NavigationModeCursorForceVisible), ro);

/* this causes problems with the embedded designer but conversion appears to work correctly without it.
  qScriptRegisterMetaType(engine, OrientationToScriptValue,  OrientationFromScriptValue);
  qScriptRegisterMetaType(engine, OrientationsToScriptValue, OrientationsFromScriptValue);
*/
  widget.setProperty("Horizontal",                QScriptValue(engine, Qt::Horizontal),             ro);
  widget.setProperty("Vertical",                  QScriptValue(engine, Qt::Vertical),               ro);

  qScriptRegisterMetaType(engine, PenCapStyleToScriptValue, PenCapStyleFromScriptValue);
  widget.setProperty("FlatCap",                   QScriptValue(engine, Qt::FlatCap),                ro);
  widget.setProperty("SquareCap",                 QScriptValue(engine, Qt::SquareCap),              ro);
  widget.setProperty("RoundCap",                  QScriptValue(engine, Qt::RoundCap),               ro);

  qScriptRegisterMetaType(engine, PenJoinStyleToScriptValue, PenJoinStyleFromScriptValue);
  widget.setProperty("MiterJoin",                 QScriptValue(engine, Qt::MiterJoin),              ro);
  widget.setProperty("BevelJoin",                 QScriptValue(engine, Qt::BevelJoin),              ro);
  widget.setProperty("RoundJoin",                 QScriptValue(engine, Qt::RoundJoin),              ro);
  widget.setProperty("SvgMiterJoin",              QScriptValue(engine, Qt::SvgMiterJoin),           ro);

  qScriptRegisterMetaType(engine, PenStyleToScriptValue, PenStyleFromScriptValue);
  widget.setProperty("NoPen",                     QScriptValue(engine, Qt::NoPen),                  ro);
  widget.setProperty("SolidLine",                 QScriptValue(engine, Qt::SolidLine),              ro);
  widget.setProperty("DashLine",                  QScriptValue(engine, Qt::DashLine),               ro);
  widget.setProperty("DotLine",                   QScriptValue(engine, Qt::DotLine),                ro);
  widget.setProperty("DashDotLine",               QScriptValue(engine, Qt::DashDotLine),            ro);
  widget.setProperty("DashDotDotLine",            QScriptValue(engine, Qt::DashDotDotLine),         ro);
  widget.setProperty("CustomDashLine",            QScriptValue(engine, Qt::CustomDashLine),         ro);

  qScriptRegisterMetaType(engine, ScreenOrientationToScriptValue,  ScreenOrientationFromScriptValue);
  qScriptRegisterMetaType(engine, ScreenOrientationsToScriptValue, ScreenOrientationsFromScriptValue);
  widget.setProperty("PrimaryOrientation",           QScriptValue(engine, Qt::PrimaryOrientation),           ro);
  widget.setProperty("LandscapeOrientation",         QScriptValue(engine, Qt::LandscapeOrientation),         ro);
  widget.setProperty("PortraitOrientation",          QScriptValue(engine, Qt::PortraitOrientation),          ro);
  widget.setProperty("InvertedLandscapeOrientation", QScriptValue(engine, Qt::InvertedLandscapeOrientation), ro);
  widget.setProperty("InvertedPortraitOrientation",  QScriptValue(engine, Qt::InvertedPortraitOrientation),  ro);

  qScriptRegisterMetaType(engine, ScrollBarPolicyToScriptValue, ScrollBarPolicyFromScriptValue);
  widget.setProperty("ScrollBarAsNeeded",         QScriptValue(engine, Qt::ScrollBarAsNeeded),      ro);
  widget.setProperty("ScrollBarAlwaysOff",        QScriptValue(engine, Qt::ScrollBarAlwaysOff),     ro);
  widget.setProperty("ScrollBarAlwaysOn",         QScriptValue(engine, Qt::ScrollBarAlwaysOn),      ro);

  qScriptRegisterMetaType(engine, ScrollPhaseToScriptValue, ScrollPhaseFromScriptValue);
#if QT_VERSION >= 0x050700
  widget.setProperty("NoScrollPhase",           QScriptValue(engine, Qt::NoScrollPhase),  ro);
#endif
  widget.setProperty("ScrollBegin",             QScriptValue(engine, Qt::ScrollBegin),    ro);
  widget.setProperty("ScrollUpdate",            QScriptValue(engine, Qt::ScrollUpdate),   ro);
  widget.setProperty("ScrollEnd",               QScriptValue(engine, Qt::ScrollEnd),      ro);

  qScriptRegisterMetaType(engine, ShortcutContextToScriptValue, ShortcutContextFromScriptValue);
  widget.setProperty("WidgetShortcut",             QScriptValue(engine, Qt::WidgetShortcut),             ro);
  widget.setProperty("WidgetWithChildrenShortcut", QScriptValue(engine, Qt::WidgetWithChildrenShortcut), ro);
  widget.setProperty("WindowShortcut",             QScriptValue(engine, Qt::WindowShortcut),             ro);
  widget.setProperty("ApplicationShortcut",        QScriptValue(engine, Qt::ApplicationShortcut),        ro);

  qScriptRegisterMetaType(engine, SizeHintToScriptValue, SizeHintFromScriptValue);
  widget.setProperty("MinimumSize",             QScriptValue(engine, Qt::MinimumSize),          ro);
  widget.setProperty("PreferredSize",           QScriptValue(engine, Qt::PreferredSize),        ro);
  widget.setProperty("MaximumSize",             QScriptValue(engine, Qt::MaximumSize),          ro);
  widget.setProperty("MinimumDescent",          QScriptValue(engine, Qt::MinimumDescent),       ro);

  qScriptRegisterMetaType(engine, SizeModeToScriptValue, SizeModeFromScriptValue);
  widget.setProperty("AbsoluteSize",            QScriptValue(engine, Qt::AbsoluteSize),         ro);
  widget.setProperty("RelativeSize",            QScriptValue(engine, Qt::RelativeSize),         ro);

  qScriptRegisterMetaType(engine, SortOrderToScriptValue, SortOrderFromScriptValue);
  widget.setProperty("AscendingOrder",          QScriptValue(engine, Qt::AscendingOrder),       ro);
  widget.setProperty("DescendingOrder",         QScriptValue(engine, Qt::DescendingOrder),      ro);

  qScriptRegisterMetaType(engine, TabFocusBehaviorToScriptValue, TabFocusBehaviorFromScriptValue);
  widget.setProperty("NoTabFocus",              QScriptValue(engine, Qt::NoTabFocus),           ro);
  widget.setProperty("TabFocusTextControls",    QScriptValue(engine, Qt::TabFocusTextControls), ro);
  widget.setProperty("TabFocusListControls",    QScriptValue(engine, Qt::TabFocusListControls), ro);
  widget.setProperty("TabFocusAllControls",     QScriptValue(engine, Qt::TabFocusAllControls),  ro);

  qScriptRegisterMetaType(engine, TextElideModeToScriptValue, TextElideModeFromScriptValue);
  widget.setProperty("ElideLeft",                  QScriptValue(engine, Qt::ElideLeft),               ro);
  widget.setProperty("ElideRight",                 QScriptValue(engine, Qt::ElideRight),              ro);
  widget.setProperty("ElideMiddle",                QScriptValue(engine, Qt::ElideMiddle),             ro);
  widget.setProperty("ElideNone",                  QScriptValue(engine, Qt::ElideNone),               ro);

  qScriptRegisterMetaType(engine, TextFlagToScriptValue, TextFlagFromScriptValue);
  widget.setProperty("TextSingleLine",             QScriptValue(engine, Qt::TextSingleLine),          ro);
  widget.setProperty("TextDontClip",               QScriptValue(engine, Qt::TextDontClip),            ro);
  widget.setProperty("TextExpandTabs",             QScriptValue(engine, Qt::TextExpandTabs),          ro);
  widget.setProperty("TextShowMnemonic",           QScriptValue(engine, Qt::TextShowMnemonic),        ro);
  widget.setProperty("TextWordWrap",               QScriptValue(engine, Qt::TextWordWrap),            ro);
  widget.setProperty("TextWrapAnywhere",           QScriptValue(engine, Qt::TextWrapAnywhere),        ro);
  widget.setProperty("TextHideMnemonic",           QScriptValue(engine, Qt::TextHideMnemonic),        ro);
  widget.setProperty("TextDontPrint",              QScriptValue(engine, Qt::TextDontPrint),           ro);
  widget.setProperty("TextIncludeTrailingSpaces",  QScriptValue(engine, Qt::TextIncludeTrailingSpaces), ro);
  widget.setProperty("TextJustificationForced",    QScriptValue(engine, Qt::TextJustificationForced),   ro);

  qScriptRegisterMetaType(engine, TextFormatToScriptValue, TextFormatFromScriptValue);
  widget.setProperty("PlainText",                  QScriptValue(engine, Qt::PlainText),               ro);
  widget.setProperty("RichText",                   QScriptValue(engine, Qt::RichText),                ro);
  widget.setProperty("AutoText",                   QScriptValue(engine, Qt::AutoText),                ro);

  qScriptRegisterMetaType(engine, TextInteractionFlagToScriptValue, TextInteractionFlagFromScriptValue);
  widget.setProperty("NoTextInteraction",          QScriptValue(engine, Qt::NoTextInteraction),         ro);
  widget.setProperty("TextSelectableByMouse",      QScriptValue(engine, Qt::TextSelectableByMouse),     ro);
  widget.setProperty("TextSelectableByKeyboard",   QScriptValue(engine, Qt::TextSelectableByKeyboard),  ro);
  widget.setProperty("LinksAccessibleByMouse",     QScriptValue(engine, Qt::LinksAccessibleByMouse),    ro);
  widget.setProperty("LinksAccessibleByKeyboard",  QScriptValue(engine, Qt::LinksAccessibleByKeyboard), ro);
  widget.setProperty("TextEditable",               QScriptValue(engine, Qt::TextEditable),              ro);
  widget.setProperty("TextEditorInteraction",      QScriptValue(engine, Qt::TextEditorInteraction),     ro);
  widget.setProperty("TextBrowserInteraction",     QScriptValue(engine, Qt::TextBrowserInteraction),    ro);

  qScriptRegisterMetaType(engine, TileRuleToScriptValue, TileRuleFromScriptValue);
  widget.setProperty("StretchTile",             QScriptValue(engine, Qt::StretchTile),   ro);
  widget.setProperty("RepeatTile",              QScriptValue(engine, Qt::RepeatTile),    ro);
  widget.setProperty("RoundTile",               QScriptValue(engine, Qt::RoundTile),     ro);

  qScriptRegisterMetaType(engine, TimeSpecToScriptValue, TimeSpecFromScriptValue);
  widget.setProperty("LocalTime",               QScriptValue(engine, Qt::LocalTime),     ro);
  widget.setProperty("UTC",                     QScriptValue(engine, Qt::UTC),           ro);
  widget.setProperty("OffsetFromUTC",           QScriptValue(engine, Qt::OffsetFromUTC), ro);
  widget.setProperty("TimeZone",                QScriptValue(engine, Qt::TimeZone),      ro);

  qScriptRegisterMetaType(engine, TimerTypeToScriptValue, TimerTypeFromScriptValue);
  widget.setProperty("PreciseTimer",               QScriptValue(engine, Qt::PreciseTimer),            ro);
  widget.setProperty("CoarseTimer",                QScriptValue(engine, Qt::CoarseTimer),             ro);
  widget.setProperty("VeryCoarseTimer",            QScriptValue(engine, Qt::VeryCoarseTimer),         ro);

  qScriptRegisterMetaType(engine, TimeSpecToScriptValue, TimeSpecFromScriptValue);
  widget.setProperty("LocalTime",                  QScriptValue(engine, Qt::LocalTime),               ro);
  widget.setProperty("UTC",                        QScriptValue(engine, Qt::UTC),                     ro);
  widget.setProperty("OffsetFromUTC",              QScriptValue(engine, Qt::OffsetFromUTC),           ro);

  qScriptRegisterMetaType(engine, ToolBarAreaToScriptValue,  ToolBarAreaFromScriptValue);
  qScriptRegisterMetaType(engine, ToolBarAreasToScriptValue, ToolBarAreasFromScriptValue);
  widget.setProperty("LeftToolBarArea",            QScriptValue(engine, Qt::LeftToolBarArea),         ro);
  widget.setProperty("RightToolBarArea",           QScriptValue(engine, Qt::RightToolBarArea),        ro);
  widget.setProperty("TopToolBarArea",             QScriptValue(engine, Qt::TopToolBarArea),          ro);
  widget.setProperty("BottomToolBarArea",          QScriptValue(engine, Qt::BottomToolBarArea),       ro);
  widget.setProperty("AllToolBarAreas",            QScriptValue(engine, Qt::AllToolBarAreas),         ro);
  widget.setProperty("NoToolBarArea",              QScriptValue(engine, Qt::NoToolBarArea),           ro);

  qScriptRegisterMetaType(engine, ToolButtonStyleToScriptValue, ToolButtonStyleFromScriptValue);
  widget.setProperty("ToolButtonIconOnly",         QScriptValue(engine, Qt::ToolButtonIconOnly),       ro);
  widget.setProperty("ToolButtonTextOnly",         QScriptValue(engine, Qt::ToolButtonTextOnly),       ro);
  widget.setProperty("ToolButtonTextBesideIcon",   QScriptValue(engine, Qt::ToolButtonTextBesideIcon), ro);
  widget.setProperty("ToolButtonTextUnderIcon",    QScriptValue(engine, Qt::ToolButtonTextUnderIcon),  ro);
  widget.setProperty("ToolButtonFollowStyle",      QScriptValue(engine, Qt::ToolButtonFollowStyle),    ro);

  qScriptRegisterMetaType(engine, TouchPointStateToScriptValue,  TouchPointStateFromScriptValue);
  qScriptRegisterMetaType(engine, TouchPointStatesToScriptValue, TouchPointStatesFromScriptValue);
  widget.setProperty("TouchPointPressed",          QScriptValue(engine, Qt::TouchPointPressed),       ro);
  widget.setProperty("TouchPointMoved",            QScriptValue(engine, Qt::TouchPointMoved),         ro);
  widget.setProperty("TouchPointStationary",       QScriptValue(engine, Qt::TouchPointStationary),    ro);
  widget.setProperty("TouchPointReleased",         QScriptValue(engine, Qt::TouchPointReleased),      ro);

  qScriptRegisterMetaType(engine, TransformationModeToScriptValue, TransformationModeFromScriptValue);
  widget.setProperty("FastTransformation",         QScriptValue(engine, Qt::FastTransformation),      ro);
  widget.setProperty("SmoothTransformation",       QScriptValue(engine, Qt::SmoothTransformation),    ro);

  qScriptRegisterMetaType(engine, UIEffectToScriptValue, UIEffectFromScriptValue);
  widget.setProperty("UI_AnimateMenu",             QScriptValue(engine, Qt::UI_AnimateMenu),          ro);
  widget.setProperty("UI_FadeMenu",                QScriptValue(engine, Qt::UI_FadeMenu),             ro);
  widget.setProperty("UI_AnimateCombo",            QScriptValue(engine, Qt::UI_AnimateCombo),         ro);
  widget.setProperty("UI_AnimateTooltip",          QScriptValue(engine, Qt::UI_AnimateTooltip),       ro);
  widget.setProperty("UI_FadeTooltip",             QScriptValue(engine, Qt::UI_FadeTooltip),          ro);
  widget.setProperty("UI_AnimateToolBox",          QScriptValue(engine, Qt::UI_AnimateToolBox),       ro);

  qScriptRegisterMetaType(engine, WhiteSpaceModeToScriptValue, WhiteSpaceModeFromScriptValue);
  widget.setProperty("WhiteSpaceNormal",           QScriptValue(engine, Qt::WhiteSpaceNormal),        ro);
  widget.setProperty("WhiteSpacePre",              QScriptValue(engine, Qt::WhiteSpacePre),           ro);
  widget.setProperty("WhiteSpaceNoWrap",           QScriptValue(engine, Qt::WhiteSpaceNoWrap),        ro);

  qScriptRegisterMetaType(engine, WidgetAttributeToScriptValue, WidgetAttributeFromScriptValue);
  widget.setProperty("WA_AcceptDrops",                    QScriptValue(engine, Qt::WA_AcceptDrops),                 ro);
  widget.setProperty("WA_AlwaysShowToolTips",             QScriptValue(engine, Qt::WA_AlwaysShowToolTips),          ro);
  widget.setProperty("WA_ContentsPropagated",             QScriptValue(engine, Qt::WA_ContentsPropagated),          ro);
  widget.setProperty("WA_CustomWhatsThis",                QScriptValue(engine, Qt::WA_CustomWhatsThis),             ro);
  widget.setProperty("WA_DeleteOnClose",                  QScriptValue(engine, Qt::WA_DeleteOnClose),               ro);
  widget.setProperty("WA_Disabled",                       QScriptValue(engine, Qt::WA_Disabled),                    ro);
  widget.setProperty("WA_DontShowOnScreen",               QScriptValue(engine, Qt::WA_DontShowOnScreen),            ro);
  widget.setProperty("WA_ForceDisabled",                  QScriptValue(engine, Qt::WA_ForceDisabled),               ro);
  widget.setProperty("WA_ForceUpdatesDisabled",           QScriptValue(engine, Qt::WA_ForceUpdatesDisabled),        ro);
  widget.setProperty("WA_GroupLeader",                    QScriptValue(engine, Qt::WA_GroupLeader),                 ro);
  widget.setProperty("WA_Hover",                          QScriptValue(engine, Qt::WA_Hover),                       ro);
  widget.setProperty("WA_InputMethodEnabled",             QScriptValue(engine, Qt::WA_InputMethodEnabled),          ro);
  widget.setProperty("WA_KeyboardFocusChange",            QScriptValue(engine, Qt::WA_KeyboardFocusChange),         ro);
  widget.setProperty("WA_KeyCompression",                 QScriptValue(engine, Qt::WA_KeyCompression),              ro);
  widget.setProperty("WA_LayoutOnEntireRect",             QScriptValue(engine, Qt::WA_LayoutOnEntireRect),          ro);
  widget.setProperty("WA_LayoutUsesWidgetRect",           QScriptValue(engine, Qt::WA_LayoutUsesWidgetRect),        ro);
  widget.setProperty("WA_MacNoClickThrough",              QScriptValue(engine, Qt::WA_MacNoClickThrough),           ro);
  widget.setProperty("WA_MacOpaqueSizeGrip",              QScriptValue(engine, Qt::WA_MacOpaqueSizeGrip),           ro);
  widget.setProperty("WA_MacShowFocusRect",               QScriptValue(engine, Qt::WA_MacShowFocusRect),            ro);
  widget.setProperty("WA_MacNormalSize",                  QScriptValue(engine, Qt::WA_MacNormalSize),               ro);
  widget.setProperty("WA_MacSmallSize",                   QScriptValue(engine, Qt::WA_MacSmallSize),                ro);
  widget.setProperty("WA_MacMiniSize",                    QScriptValue(engine, Qt::WA_MacMiniSize),                 ro);
  widget.setProperty("WA_MacVariableSize",                QScriptValue(engine, Qt::WA_MacVariableSize),             ro);
  widget.setProperty("WA_MacBrushedMetal",                QScriptValue(engine, Qt::WA_MacBrushedMetal),             ro);
  widget.setProperty("WA_Mapped",                         QScriptValue(engine, Qt::WA_Mapped),                      ro);
  widget.setProperty("WA_MouseNoMask",                    QScriptValue(engine, Qt::WA_MouseNoMask),                 ro);
  widget.setProperty("WA_MouseTracking",                  QScriptValue(engine, Qt::WA_MouseTracking),               ro);
  widget.setProperty("WA_Moved",                          QScriptValue(engine, Qt::WA_Moved),                       ro);
  widget.setProperty("WA_MSWindowsUseDirect3D",           QScriptValue(engine, Qt::WA_MSWindowsUseDirect3D),        ro);
  widget.setProperty("WA_NoBackground",                   QScriptValue(engine, Qt::WA_NoBackground),                ro);
  widget.setProperty("WA_NoChildEventsForParent",         QScriptValue(engine, Qt::WA_NoChildEventsForParent),      ro);
  widget.setProperty("WA_NoChildEventsFromChildren",      QScriptValue(engine, Qt::WA_NoChildEventsFromChildren),   ro);
  widget.setProperty("WA_NoMouseReplay",                  QScriptValue(engine, Qt::WA_NoMouseReplay),               ro);
  widget.setProperty("WA_NoMousePropagation",             QScriptValue(engine, Qt::WA_NoMousePropagation),          ro);
  widget.setProperty("WA_TransparentForMouseEvents",      QScriptValue(engine, Qt::WA_TransparentForMouseEvents),   ro);
  widget.setProperty("WA_NoSystemBackground",             QScriptValue(engine, Qt::WA_NoSystemBackground),          ro);
  widget.setProperty("WA_OpaquePaintEvent",               QScriptValue(engine, Qt::WA_OpaquePaintEvent),            ro);
  widget.setProperty("WA_OutsideWSRange",                 QScriptValue(engine, Qt::WA_OutsideWSRange),              ro);
  widget.setProperty("WA_PaintOnScreen",                  QScriptValue(engine, Qt::WA_PaintOnScreen),               ro);
  widget.setProperty("WA_PaintUnclipped",                 QScriptValue(engine, Qt::WA_PaintUnclipped),              ro);
  widget.setProperty("WA_PendingMoveEvent",               QScriptValue(engine, Qt::WA_PendingMoveEvent),            ro);
  widget.setProperty("WA_PendingResizeEvent",             QScriptValue(engine, Qt::WA_PendingResizeEvent),          ro);
  widget.setProperty("WA_QuitOnClose",                    QScriptValue(engine, Qt::WA_QuitOnClose),                 ro);
  widget.setProperty("WA_Resized",                        QScriptValue(engine, Qt::WA_Resized),                     ro);
  widget.setProperty("WA_RightToLeft",                    QScriptValue(engine, Qt::WA_RightToLeft),                 ro);
  widget.setProperty("WA_SetCursor",                      QScriptValue(engine, Qt::WA_SetCursor),                   ro);
  widget.setProperty("WA_SetFont",                        QScriptValue(engine, Qt::WA_SetFont),                     ro);
  widget.setProperty("WA_SetPalette",                     QScriptValue(engine, Qt::WA_SetPalette),                  ro);
  widget.setProperty("WA_SetStyle",                       QScriptValue(engine, Qt::WA_SetStyle),                    ro);
  widget.setProperty("WA_ShowModal",                      QScriptValue(engine, Qt::WA_ShowModal),                   ro);
  widget.setProperty("WA_StaticContents",                 QScriptValue(engine, Qt::WA_StaticContents),              ro);
  widget.setProperty("WA_StyleSheet",                     QScriptValue(engine, Qt::WA_StyleSheet),                  ro);
#if QT_VERSION >= 0x050900
  widget.setProperty("WA_TabletTracking",                 QScriptValue(engine, Qt::WA_TabletTracking),              ro);
#endif
  widget.setProperty("WA_TranslucentBackground",          QScriptValue(engine, Qt::WA_TranslucentBackground),       ro);
  widget.setProperty("WA_UnderMouse",                     QScriptValue(engine, Qt::WA_UnderMouse),                  ro);
  widget.setProperty("WA_UpdatesDisabled",                QScriptValue(engine, Qt::WA_UpdatesDisabled),             ro);
  widget.setProperty("WA_WindowModified",                 QScriptValue(engine, Qt::WA_WindowModified),              ro);
  widget.setProperty("WA_WindowPropagation",              QScriptValue(engine, Qt::WA_WindowPropagation),           ro);
  widget.setProperty("WA_MacAlwaysShowToolWindow",        QScriptValue(engine, Qt::WA_MacAlwaysShowToolWindow),     ro);
  widget.setProperty("WA_SetLocale",                      QScriptValue(engine, Qt::WA_SetLocale),                   ro);
  widget.setProperty("WA_StyledBackground",               QScriptValue(engine, Qt::WA_StyledBackground),            ro);
  widget.setProperty("WA_ShowWithoutActivating",          QScriptValue(engine, Qt::WA_ShowWithoutActivating),       ro);
  widget.setProperty("WA_NativeWindow",                   QScriptValue(engine, Qt::WA_NativeWindow),                ro);
  widget.setProperty("WA_DontCreateNativeAncestors",      QScriptValue(engine, Qt::WA_DontCreateNativeAncestors),   ro);
  widget.setProperty("WA_X11NetWmWindowTypeDesktop",      QScriptValue(engine, Qt::WA_X11NetWmWindowTypeDesktop),   ro);
  widget.setProperty("WA_X11NetWmWindowTypeDock",         QScriptValue(engine, Qt::WA_X11NetWmWindowTypeDock),      ro);
  widget.setProperty("WA_X11NetWmWindowTypeToolBar",      QScriptValue(engine, Qt::WA_X11NetWmWindowTypeToolBar),   ro);
  widget.setProperty("WA_X11NetWmWindowTypeMenu",         QScriptValue(engine, Qt::WA_X11NetWmWindowTypeMenu),      ro);
  widget.setProperty("WA_X11NetWmWindowTypeUtility",      QScriptValue(engine, Qt::WA_X11NetWmWindowTypeUtility),   ro);
  widget.setProperty("WA_X11NetWmWindowTypeSplash",       QScriptValue(engine, Qt::WA_X11NetWmWindowTypeSplash),    ro);
  widget.setProperty("WA_X11NetWmWindowTypeDialog",       QScriptValue(engine, Qt::WA_X11NetWmWindowTypeDialog),    ro);
  widget.setProperty("WA_X11NetWmWindowTypeDropDownMenu", QScriptValue(engine, Qt::WA_X11NetWmWindowTypeDropDownMenu), ro);
  widget.setProperty("WA_X11NetWmWindowTypePopupMenu",    QScriptValue(engine, Qt::WA_X11NetWmWindowTypePopupMenu), ro);
  widget.setProperty("WA_X11NetWmWindowTypeToolTip",      QScriptValue(engine, Qt::WA_X11NetWmWindowTypeToolTip),   ro);
  widget.setProperty("WA_X11NetWmWindowTypeNotification", QScriptValue(engine, Qt::WA_X11NetWmWindowTypeNotification), ro);
  widget.setProperty("WA_X11NetWmWindowTypeCombo",        QScriptValue(engine, Qt::WA_X11NetWmWindowTypeCombo),     ro);
  widget.setProperty("WA_X11NetWmWindowTypeDND",          QScriptValue(engine, Qt::WA_X11NetWmWindowTypeDND),       ro);
  widget.setProperty("WA_MacFrameworkScaled",             QScriptValue(engine, Qt::WA_MacFrameworkScaled),          ro);
  widget.setProperty("WA_AcceptTouchEvents",               QScriptValue(engine, Qt::WA_AcceptTouchEvents),               ro);
  widget.setProperty("WA_TouchPadAcceptSingleTouchEvents", QScriptValue(engine, Qt::WA_TouchPadAcceptSingleTouchEvents), ro);
  widget.setProperty("WA_X11DoNotAcceptFocus",             QScriptValue(engine, Qt::WA_X11DoNotAcceptFocus),             ro);
  widget.setProperty("WA_AlwaysStackOnTop",                QScriptValue(engine, Qt::WA_AlwaysStackOnTop),                ro);


  qScriptRegisterMetaType(engine, WindowFrameSectionToScriptValue, WindowFrameSectionFromScriptValue);
  widget.setProperty("NoSection",                 QScriptValue(engine, Qt::NoSection),              ro);
  widget.setProperty("LeftSection",               QScriptValue(engine, Qt::LeftSection),            ro);
  widget.setProperty("TopLeftSection",            QScriptValue(engine, Qt::TopLeftSection),         ro);
  widget.setProperty("TopSection",                QScriptValue(engine, Qt::TopSection),             ro);
  widget.setProperty("TopRightSection",           QScriptValue(engine, Qt::TopRightSection),        ro);
  widget.setProperty("RightSection",              QScriptValue(engine, Qt::RightSection),           ro);
  widget.setProperty("BottomRightSection",        QScriptValue(engine, Qt::BottomRightSection),     ro);
  widget.setProperty("BottomSection",             QScriptValue(engine, Qt::BottomSection),          ro);
  widget.setProperty("BottomLeftSection",         QScriptValue(engine, Qt::BottomLeftSection),      ro);
  widget.setProperty("TitleBarArea",              QScriptValue(engine, Qt::TitleBarArea),           ro);

  qScriptRegisterMetaType(engine, WindowModalityToScriptValue, WindowModalityFromScriptValue);
  widget.setProperty("NonModal",                  QScriptValue(engine, Qt::NonModal),               ro);
  widget.setProperty("WindowModal",               QScriptValue(engine, Qt::WindowModal),            ro);
  widget.setProperty("ApplicationModal",          QScriptValue(engine, Qt::ApplicationModal),       ro);

  qScriptRegisterMetaType(engine, WindowStateToScriptValue, WindowStateFromScriptValue);
  widget.setProperty("WindowNoState",             QScriptValue(engine, Qt::WindowNoState),          ro);
  widget.setProperty("WindowMinimized",           QScriptValue(engine, Qt::WindowMinimized),        ro);
  widget.setProperty("WindowMaximized",           QScriptValue(engine, Qt::WindowMaximized),        ro);
  widget.setProperty("WindowFullScreen",          QScriptValue(engine, Qt::WindowFullScreen),       ro);
  widget.setProperty("WindowActive",              QScriptValue(engine, Qt::WindowActive),           ro);

  qScriptRegisterMetaType(engine, WindowTypeToScriptValue,  WindowTypeFromScriptValue);
  qScriptRegisterMetaType(engine, WindowFlagsToScriptValue, WindowFlagsFromScriptValue);
  widget.setProperty("Widget",                         QScriptValue(engine, Qt::Widget),                       ro);
  widget.setProperty("Window",                         QScriptValue(engine, Qt::Window),                       ro);
  widget.setProperty("Dialog",                         QScriptValue(engine, Qt::Dialog),                       ro);
  widget.setProperty("Sheet",                          QScriptValue(engine, Qt::Sheet),                        ro);
  widget.setProperty("Drawer",                         QScriptValue(engine, Qt::Drawer),                       ro);
  widget.setProperty("Popup",                          QScriptValue(engine, Qt::Popup),                        ro);
  widget.setProperty("Tool",                           QScriptValue(engine, Qt::Tool),                         ro);
  widget.setProperty("ToolTip",                        QScriptValue(engine, Qt::ToolTip),                      ro);
  widget.setProperty("SplashScreen",                   QScriptValue(engine, Qt::SplashScreen),                 ro);
  widget.setProperty("Desktop",                        QScriptValue(engine, Qt::Desktop),                      ro);
  widget.setProperty("SubWindow",                      QScriptValue(engine, Qt::SubWindow),                    ro);
  widget.setProperty("ForeignWindow",                  QScriptValue(engine, Qt::ForeignWindow),                ro);
  widget.setProperty("CoverWindow",                    QScriptValue(engine, Qt::CoverWindow),                  ro);
  widget.setProperty("MSWindowsFixedSizeDialogHint",   QScriptValue(engine, Qt::MSWindowsFixedSizeDialogHint), ro);
  widget.setProperty("MSWindowsOwnDC",                 QScriptValue(engine, Qt::MSWindowsOwnDC),               ro);
  widget.setProperty("BypassWindowManagerHint",        QScriptValue(engine, Qt::BypassWindowManagerHint),      ro);
  widget.setProperty("X11BypassWindowManagerHint",     QScriptValue(engine, Qt::X11BypassWindowManagerHint),   ro);
  widget.setProperty("FramelessWindowHint",            QScriptValue(engine, Qt::FramelessWindowHint),          ro);
  widget.setProperty("NoDropShadowWindowHint",         QScriptValue(engine, Qt::NoDropShadowWindowHint),       ro);
  widget.setProperty("CustomizeWindowHint",            QScriptValue(engine, Qt::CustomizeWindowHint),          ro);
  widget.setProperty("WindowTitleHint",                QScriptValue(engine, Qt::WindowTitleHint),              ro);
  widget.setProperty("WindowSystemMenuHint",           QScriptValue(engine, Qt::WindowSystemMenuHint),         ro);
  widget.setProperty("WindowMinimizeButtonHint",       QScriptValue(engine, Qt::WindowMinimizeButtonHint),     ro);
  widget.setProperty("WindowMaximizeButtonHint",       QScriptValue(engine, Qt::WindowMaximizeButtonHint),     ro);
  widget.setProperty("WindowMinMaxButtonsHint",        QScriptValue(engine, Qt::WindowMinMaxButtonsHint),      ro);
  widget.setProperty("WindowCloseButtonHint",          QScriptValue(engine, Qt::WindowCloseButtonHint),        ro);
  widget.setProperty("WindowContextHelpButtonHint",    QScriptValue(engine, Qt::WindowContextHelpButtonHint),  ro);
  widget.setProperty("MacWindowToolBarButtonHint",     QScriptValue(engine, Qt::MacWindowToolBarButtonHint),   ro);
  widget.setProperty("WindowFullscreenButtonHint",     QScriptValue(engine, Qt::WindowFullscreenButtonHint),   ro);
  widget.setProperty("BypassGraphicsProxyWidget",      QScriptValue(engine, Qt::BypassGraphicsProxyWidget),    ro);
  widget.setProperty("WindowShadeButtonHint",          QScriptValue(engine, Qt::WindowShadeButtonHint),        ro);
  widget.setProperty("WindowStaysOnTopHint",           QScriptValue(engine, Qt::WindowStaysOnTopHint),         ro);
  widget.setProperty("WindowStaysOnBottomHint",        QScriptValue(engine, Qt::WindowStaysOnBottomHint),      ro);

  widget.setProperty("WindowTransparentForInput",        QScriptValue(engine, Qt::WindowTransparentForInput),      ro);
  widget.setProperty("WindowOverridesSystemGestures",        QScriptValue(engine, Qt::WindowOverridesSystemGestures),      ro);
  widget.setProperty("WindowDoesNotAcceptFocus",        QScriptValue(engine, Qt::WindowDoesNotAcceptFocus),      ro);
  widget.setProperty("MaximizeUsingFullscreenGeometryHint",        QScriptValue(engine, Qt::MaximizeUsingFullscreenGeometryHint),      ro);

#if QT_VERSION < 0x050800
  widget.setProperty("WindowOkButtonHint",             QScriptValue(engine, Qt::WindowOkButtonHint),           ro);
  widget.setProperty("WindowCancelButtonHint",         QScriptValue(engine, Qt::WindowCancelButtonHint),       ro);
#endif
  widget.setProperty("WindowType_Mask",                QScriptValue(engine, Qt::WindowType_Mask),              ro);

  engine->globalObject().setProperty("Qt", widget, ro);
}
