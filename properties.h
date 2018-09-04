#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <QVariant>
#include <QColor>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>

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
    Texture,
    File,
    List
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
};

struct TextureProperty : public Property
{
    QString value;
    QString toggleValue;
    bool toggle;

    TextureProperty () {
        type = PropertyType::Texture;
        toggle = false;
    }

    QVariant getValue() {
        return value;
    }

    void setValue(QVariant val) {
        value = val.toString();
        toggle = !value.isEmpty();
    }

    QString getUniformName() override
    {
        return QString("input_")+name;
    }

    QString getUniformString() override
    {
        return QString("uniform sampler2D ") + getUniformName();
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
};

#endif // PROPERTIES_H
