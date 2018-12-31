#include <QUuid>
#include <QString>
#include <QColor>

#include "nodemodel.h"
#include "socketmodel.h"
#include "../core/guidhelper.h"

NodeModel::NodeModel()
{
	id = GuidHelper::createGuid();
	widget = nullptr;
	connect(this, &NodeModel::titleColorChanged, [=]() {
		setNodeTitleColor();
	});

	enablePreview = false;

	x = 0;
	y = 0;
}

void NodeModel::updateStyle()
{
	widget->setStyleSheet("QMenu{	background: rgba(26,26,26,.9); color: rgba(250,250, 250,.9);}"
		"QMenu::item{padding: 2px 5px 2px 20px;	}"
		"QMenu::item:hover{	background: rgba(40,128, 185,.9);}"
		"QMenu::item:selected{	background: rgba(40,128, 185,.9);}"
		"QCheckBox {   spacing: 2px 5px;}"
		"QCheckBox::indicator {   width: 28px;   height: 28px; }"
		"QCheckBox::indicator::unchecked {	image: url(:/icons/check-unchecked.png);}"
		"QCheckBox::indicator::checked {		image: url(:/icons/check-checked.png);}"
		"QLineEdit {	border: 0;	background: #292929;	padding: 6px;	margin: 0;}"
		"QToolButton {	background: #1E1E1E;	border: 0;	padding: 6px;}"
		"QToolButton:pressed {	background: #111;}"
		"QToolButton:hover {	background: #404040;}"
		"QDoubleSpinBox, QSpinBox {	border-radius: 1px;	padding: 6px;	background: #292929;}"
		"QDoubleSpinBox::up-arrow, QSpinBox::up-arrow {	width:0;}"
		"QDoubleSpinBox::up-button, QSpinBox::up-button, QDoubleSpinBox::down-button, QSpinBox::down-button {	width:0;}"
		"QComboBox:editable {}"
		"QComboBox QAbstractItemView::item {    show-decoration-selected: 1;}"
		"QComboBox QAbstractItemView::item {    padding: 6px;}"
		"QListView::item:selected {    background: #404040;}"
		"QComboBox:!editable, QComboBox::drop-down:editable {     background: #1A1A1A;}"
		"QComboBox:!editable:on, QComboBox::drop-down:editable:on {    background: #1A1A1A;}"
		"QComboBox QAbstractItemView {    background-color: #1A1A1A;    selection-background-color: #404040;    border: 0;    outline: none;}"
		"QComboBox QAbstractItemView::item {    border: none;    padding-left: 5px;}"
		"QComboBox QAbstractItemView::item:selected {    background: #404040;    padding-left: 5px;}"
		"QComboBox::drop-down {    subcontrol-origin: padding;    subcontrol-position: top right;    width: 18px;    border-left-width: 1px;}"
		"QComboBox::down-arrow {    image: url(:/icons/down_arrow_check.png);	width: 18px;	height: 14px;} "
		"QComboBox::down-arrow:!enabled {    image: url(:/icons/down_arrow_check_disabled.png);    width: 18px;    height: 14px;}"
		"QLabel{}"
		"QPushButton{padding : 3px; }"
	);
}

void NodeModel::addInputSocket(SocketModel *sock)
{
	inSockets.append(sock);
	sock->setNode(this);
}

void NodeModel::addOutputSocket(SocketModel *sock)
{
	outSockets.append(sock);
	sock->setNode(this);
}

void NodeModel::setWidget(QWidget * wid)
{
	widget = wid;
	updateStyle();
}

QString NodeModel::getValueFromInputSocket(int index)
{
	auto sock = inSockets[index];
	if (sock->hasConnection()) {
		//return sock->getConnectedSocket()->getVarName();

		// converts the var before sending it back
		return sock->getConnectedSocket()->convertVarTo(sock);
	}

	return sock->getValue();
}

QString NodeModel::getOutputSocketVarName(int index)
{
	auto sock = outSockets[index];
	return sock->getVarName();
}

NodeGraph *NodeModel::getGraph() const
{
	return graph;
}

void NodeModel::setGraph(NodeGraph *value)
{
	graph = value;
}

QColor NodeModel::setNodeTitleColor()
{
	switch (nodeType) {
	case NodeType::Input:
		icon.addPixmap({":/icons/input.png"	});
		return titleColor = QColor(0, 121, 107);
		break;
	case NodeType::Math:
		icon.addPixmap({ ":/icons/math.svg" });
		return titleColor = QColor(25,118,210);
		break;
	case NodeType::Properties:
		icon.addPixmap({ ":/icons/properties.png" });
		return titleColor = QColor(230, 74, 25);
		break;
	case NodeType::Constants:
		icon.addPixmap({ ":/icons/constants.png" });
		return titleColor = QColor(150, 24, 35);
		break;
	default:
		return titleColor = QColor(0, 0, 0, 0);
		break;
	}

}

QString NodeModel::getEnumString(NodeType type) {

	switch (type) {
	case NodeType::Input:
		return "Input";
	case NodeType::Math:
		return "Math";
	case NodeType::Properties:
		return "Properties";
	case NodeType::Constants:
		return "Constants";
	case NodeType::Object:
		return "Object";
	case NodeType::Texture:
		return "Texture";
	case NodeType::Vector:
		return "Vector";
	case NodeType::Vertex:
		return "Vertex";
	default:
		return "";
	}
}

void NodeModel::setNodeType(NodeType type)
{
	nodeType = type;
	emit titleColorChanged();

}