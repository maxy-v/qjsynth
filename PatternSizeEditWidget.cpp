#include "PatternSizeEditWidget.h"

PatternSizeEditWidget::PatternSizeEditWidget(QWidget* a_parent)
	: QSpinBox(a_parent)
{
	setSingleStep(Properties::Core::FramesPerSecond);
	setMinimum( Properties::Pattern::MinSize );
	setMaximum( Properties::Pattern::MaxSize );
	reset();
}

void PatternSizeEditWidget::reset()
{
	setValue( Properties::Pattern::DefaultSize );
}

QString PatternSizeEditWidget::textFromValue(int a_value) const
{
	int seconds = a_value/Properties::Core::FramesPerSecond;
	int frames  = a_value-seconds*Properties::Core::FramesPerSecond;
	int minutes = seconds/Properties::Core::SecondsInMinute;
	seconds -= minutes*Properties::Core::SecondsInMinute;

	return QString("%1:%2:%3")
		.arg(minutes, 2, 10, QChar{'0'})
		.arg(seconds, 2, 10, QChar{'0'})
		.arg(frames , 2, 10, QChar{'0'});
}

int PatternSizeEditWidget::valueFromText(const QString& a_text) const
{
	int value = 0;
	QStringList parts = a_text.split(':');
	if (parts.size() == 3)
	{
		value += parts[0].toInt()*Properties::Core::FramesPerSecond*Properties::Core::SecondsInMinute;
		value += parts[1].toInt()*Properties::Core::FramesPerSecond;
		value += parts[2].toInt();
	}
	return value;
}

QValidator::State PatternSizeEditWidget::validate(QString& a_text, int& a_pos) const
{
	Q_UNUSED(a_pos);
	QStringList parts = a_text.split(':');
	if (parts.size() == 3)
		return QValidator::Acceptable;
	return QValidator::Invalid;
}

void PatternSizeEditWidget::wheelEvent(QWheelEvent* a_event)
{
	if (a_event->modifiers() == Qt::ShiftModifier)
	{
		if (a_event->angleDelta().y() > 0)
			setValue(value() + 1);
		else
		if (a_event->angleDelta().y() < 0)
			setValue(value() - 1);
	}
	else
		QSpinBox::wheelEvent(a_event);
}