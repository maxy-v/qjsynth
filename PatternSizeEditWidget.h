#pragma once
#include <QSpinBox>
#include <QWheelEvent>
#include "properties.h"

class PatternSizeEditWidget: public QSpinBox
{
public:
	PatternSizeEditWidget(QWidget* a_parent = nullptr);
	void reset();

protected:
	virtual QString textFromValue(int a_value) const override;
	virtual int 	valueFromText(const QString& a_text) const override;
	virtual QValidator::State validate(QString& a_text, int& a_pos) const override;
	virtual void wheelEvent(QWheelEvent* a_event) override;
};