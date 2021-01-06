#include "LibraryDelegate.h"

LibraryDelegate::LibraryDelegate(QObject* a_parent)
	: QStyledItemDelegate(a_parent)
{
}

QWidget* LibraryDelegate::createEditor(QWidget* a_parent, const QStyleOptionViewItem& a_option, const QModelIndex& a_index) const
{
	switch(a_index.column())
	{
	case LibraryModel::Column::Name:
		return QStyledItemDelegate::createEditor(a_parent, a_option, a_index);
	case LibraryModel::Column::Color:
		{
			QColor color = a_index.data(Qt::BackgroundRole).value<QColor>();
			QColorDialog* dialog = new QColorDialog(color, a_parent);
			dialog->setModal(true);
			QtTools::centerWidgetOnCursor(dialog);
			connect(dialog, &QColorDialog::accepted, this, &LibraryDelegate::onAccept);
			connect(dialog, &QColorDialog::rejected, this, &LibraryDelegate::onReject);
			return dialog;
		}
	}

	return nullptr;
}

void LibraryDelegate::setModelData(QWidget* a_editor, QAbstractItemModel* a_model, const QModelIndex& a_index) const
{
	switch(a_index.column())
	{
		case LibraryModel::Column::Color:
		{
			QColorDialog* dialog = qobject_cast<QColorDialog*>(a_editor);
			if (dialog->selectedColor().isValid())
				a_model->setData(a_index, dialog->selectedColor(), Qt::BackgroundColorRole);
			return;
		}
		default:
			QStyledItemDelegate::setModelData(a_editor, a_model, a_index);
	}
}

void LibraryDelegate::paint(QPainter* a_painter, const QStyleOptionViewItem& a_option, const QModelIndex& a_index) const
{
	if (a_index.column() != LibraryModel::Column::Name)
		QStyledItemDelegate::paint(a_painter, a_option, a_index);
	else
	{
		a_painter->save();

		QStyleOptionViewItem option = a_option;
		initStyleOption(&option, a_index);
		option.backgroundBrush = Qt::NoBrush;

		// Draw as usual
		auto widget = a_option.widget;
		Q_ASSERT(widget != nullptr);
		widget->style()->drawControl(QStyle::CE_ItemViewItem, &option, a_painter, widget);

		// Draw item color icon on the right side
		const QModelIndex color_index = a_index.siblingAtColumn( LibraryModel::Column::Color );
		QColor color = color_index.data(Qt::BackgroundRole).value<QColor>();
		if (color.isValid())
		{
			a_painter->setPen(Qt::NoPen);
			a_painter->setBrush(color);
			int offset = static_cast<int>( a_option.rect.height() * 0.2f );
			QRect position = a_option.rect.adjusted(offset,offset,-offset,-offset);
			position.setX( position.width() - position.height() );
			a_painter->drawRect(position);
		}

		a_painter->restore();
	}
}

void LibraryDelegate::onAccept()
{
	QColorDialog* dialog = qobject_cast<QColorDialog*>(sender());
	emit closeEditor(dialog);
	emit commitData(dialog);
	dialog->deleteLater();
}

void LibraryDelegate::onReject()
{
	QColorDialog* dialog = qobject_cast<QColorDialog*>(sender());
	emit closeEditor(dialog);
	dialog->deleteLater();
}
