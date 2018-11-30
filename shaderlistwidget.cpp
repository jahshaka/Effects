#include "shaderlistwidget.h"
#include <QDropEvent>
#include <QMimeData>
#include <QDebug>
#include <QUuid>

#include "core/project.h"


ShaderListWidget::ShaderListWidget() : ListWidget()
{
	setAcceptDrops(true);
	viewport()->setAcceptDrops(true);

}


ShaderListWidget::~ShaderListWidget()
{

}


void ShaderListWidget::dropEvent(QDropEvent * event)
{

	if(event->mimeData()->data("MODEL_TYPE_ROLE").toInt() == (int)ModelTypes::Material)
	{
		event->accept();
		event->setDropAction(Qt::CopyAction);

		AssetRecord data;

		data.name = event->mimeData()->text();


		QListWidgetItem *item = new QListWidgetItem;
		item->setFlags(item->flags() | Qt::ItemIsEditable);
		item->setSizeHint({90,90});
		item->setTextAlignment(Qt::AlignCenter);
		item->setIcon(QIcon(":/icons/icons8-file-72.png"));
		item->setText(event->mimeData()->text());

		auto genGuid = []() {
			auto id = QUuid::createUuid();
			auto guid = id.toString().remove(0, 1);
			guid.chop(1);
			return guid;
		};

		const QString assetGuid = genGuid();

		item->setData(MODEL_GUID_ROLE, assetGuid);
		item->setData(MODEL_PARENT_ROLE, "");
		item->setData(MODEL_ITEM_TYPE, MODEL_ASSET);
		item->setData(MODEL_TYPE_ROLE, static_cast<int>(ModelTypes::Material));

		emit itemDropped(item);






	}
	else	event->ignore();


	ListWidget::dropEvent(event);
}

void ShaderListWidget::dragMoveEvent(QDragMoveEvent * event)
{
	event->acceptProposedAction();
//	ListWidget::dragMoveEvent(event);
}

void ShaderListWidget::dragEnterEvent(QDragEnterEvent * event)
{
	event->acceptProposedAction();
//	ListWidget::dragEnterEvent(event);
}

void ShaderListWidget::dragLeaveEvent(QDragLeaveEvent * event)
{
	qDebug() << "drag leaved";
//	ListWidget::dragLeaveEvent(event);
}
