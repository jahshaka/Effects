#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <QVariant>
#include <QColor>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QJsonObject>
#include <QJsonValue>

enum class PropertyType
{
    None,
    Bool,
    Int,
    Float,
    Vec2,
    Vec3,
    Vec4,
    Color,
    Texture
};

struct Property
{
    unsigned            id;
    QString             displayName;
    QString             name;
    //QString             uniform;
    PropertyType        type;

    virtual QVariant    getValue() = 0;
    virtual void        setValue(QVariant val) = 0;

    virtual QString getUniformString() = 0;
    virtual QString getUniformName() = 0;

    virtual QJsonObject serialize();
    virtual void deserialize(const QJsonObject& obj);

    static Property* parse(const QJsonObject& obj);
};

class PropertyListener
{
public:
    virtual void onPropertyChanged(Property*) = 0;
    virtual void onPropertyChangeStart(Property*) = 0;
    virtual void onPropertyChangeEnd(Property*) = 0;
};

struct BoolProperty : public Property
{
    bool value;

    BoolProperty () {
        type = PropertyType::Bool;
        value = false;
    }

    QVariant getValue() {
        return value;
    }

    void setValue(QVariant val) {
        value = val.toBool();
    }

    QString getUniformName() override
    {
        return QString("input_")+name;
    }

    QString getUniformString() override
    {
        return QString("uniform bool ") + getUniformName();
    }

    QJsonObject serialize() override
    {
        auto obj = Property::serialize();
        obj["value"] = value?"true":"false";
        return obj;
    }

    void deserialize(const QJsonObject& obj) override
    {
        value = obj["value"].toBool();
    }
};

struct IntProperty : public Property
{
    int value;
    int minValue;
    int maxValue;

    IntProperty() {
        type = PropertyType::Int;
    }

    QVariant getValue() {
        return value;
    }

    void setValue(QVariant val) {
        value = val.toInt();
    }

    QString getUniformName() override
    {
        return QString("input_")+name;
    }

    QString getUniformString() override
    {
        return QString("uniform int ") + getUniformName();
    }

    QJsonObject serialize() override
    {
        auto obj = Property::serialize();
        obj["value"] = value;
        return obj;
    }

    void deserialize(const QJsonObject& obj) override
    {
        value = obj["value"].toInt();
    }
};

struct FloatProperty : public Property
{
    float value;
    float minValue;
    float maxValue;

    FloatProperty() {
        value = 0;
        type = PropertyType::Float;
    }

    QVariant getValue() {
        return QVariant::fromValue(value);
    }

    void setValue(QVariant val) {
        value = val.toFloat();
    }

    QString getUniformName() override
    {
        return QString("input_")+name;
    }

    QString getUniformString() override
    {
        return QString("uniform float ") + getUniformName();
    }

    QJsonObject serialize() override
    {
        auto obj = Property::serialize();
        obj["value"] = value;
        return obj;
    }

    void deserialize(const QJsonObject& obj) override
    {
        value = (float)obj["value"].toDouble();
    }
};

struct ColorProperty : public Property
{
    QColor value;

    ColorProperty () {
        type = PropertyType::Color;
    }

    QVariant getValue() {
        return value;
    }

    void setValue(QVariant val) {
        value = val.value<QColor>();
    }

    QString getUniformName() override
    {
        return QString("input_")+name;
    }

    QString getUniformString() override
    {
        return QString("uniform vec4 ") + getUniformName();
    }

    QJsonObject serialize() override
    {
        auto obj = Property::serialize();
        QJsonObject colObj;
        colObj["r"] = value.red();
        colObj["g"] = value.green();
        colObj["b"] = value.blue();
        colObj["a"] = value.alpha();
        obj["value"] = colObj;

        return obj;
    }

    void deserialize(const QJsonObject& obj) override
    {
        value = obj["value"];
    }
};

struct TextureProperty : public Property
{
    QString value;

    TextureProperty () {
        type = PropertyType::Texture;
    }

    QVariant getValue() {
        return value;
    }

    void setValue(QVariant val) {
        value = val.toString();
    }

    QString getUniformName() override
    {
        return QString("input_")+name;
    }

    QString getUniformString() override
    {
        return QString("uniform sampler2D ") + getUniformName();
    }

    QJsonObject serialize() override
    {
        auto obj = Property::serialize();
        obj["value"] = value;
        return obj;
    }

    void deserialize(const QJsonObject& obj) override
    {
        value = obj["value"].toString();
    }
};

struct Vec2Property : public Property
{
    QVector2D value;

    Vec2Property() {
        type = PropertyType::Vec2;
    }

    QVariant getValue() {
        return value;
    }

    void setValue(QVariant val) {
        value = val.value<QVector2D>();
    }

    QString getUniformName() override
    {
        return QString("input_")+name;
    }

    QString getUniformString() override
    {
        return QString("uniform vec2 ") + getUniformName();
    }

    QJsonObject serialize() override
    {
        auto obj = Property::serialize();
        QJsonObject vec;
        vec["x"] = vec.x();
        vec["y"] = vec.y();

        obj["value"] = vec;
        return obj;
    }

    void deserialize(const QJsonObject& obj) override
    {
        value = obj["value"];
    }
};

struct Vec3Property : public Property
{
    QVector3D value;

    Vec3Property() {
        type = PropertyType::Vec3;
    }

    QVariant getValue() {
        return value;
    }

    void setValue(QVariant val) {
        value = val.value<QVector3D>();
    }

    QString getUniformName() override
    {
        return QString("input_")+name;
    }

    QString getUniformString() override
    {
        return QString("uniform vec3 ") + getUniformName();
    }

    QJsonObject serialize() override
    {
        auto obj = Property::serialize();
        QJsonObject vec;
        vec["x"] = vec.x();
        vec["y"] = vec.y();
        vec["z"] = vec.z();

        obj["value"] = vec;
        return obj;
    }

    void deserialize(const QJsonObject& obj) override
    {
        value = obj["value"];
    }
};

struct Vec4Property : public Property
{
    QVector4D value;

    Vec4Property() {
        type = PropertyType::Vec4;
    }

    QVariant getValue() {
        return value;
    }

    void setValue(QVariant val) {
        value = val.value<QVector4D>();
    }

    QString getUniformName() override
    {
        return QString("input_")+name;
    }

    QString getUniformString() override
    {
        return QString("uniform vec4 ") + getUniformName();
    }

    QJsonObject serialize() override
    {
        auto obj = Property::serialize();
        QJsonObject vec;
        vec["x"] = vec.x();
        vec["y"] = vec.y();
        vec["z"] = vec.z();
        vec["w"] = vec.w();

        obj["value"] = vec;
        return obj;
    }

    void deserialize(const QJsonObject& obj) override
    {
        value = obj["value"];
    }
};

#endif // PROPERTIES_H
