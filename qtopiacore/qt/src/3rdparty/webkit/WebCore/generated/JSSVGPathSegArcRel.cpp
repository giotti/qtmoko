/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "config.h"


#if ENABLE(SVG)

#include "SVGElement.h"
#include "JSSVGPathSegArcRel.h"

#include <wtf/GetPtr.h>

#include "SVGPathSegArc.h"

#include <runtime/JSNumberCell.h>

using namespace JSC;

namespace WebCore {

ASSERT_CLASS_FITS_IN_CELL(JSSVGPathSegArcRel)

/* Hash table */

static const HashTableValue JSSVGPathSegArcRelTableValues[8] =
{
    { "x", DontDelete, (intptr_t)jsSVGPathSegArcRelX, (intptr_t)setJSSVGPathSegArcRelX },
    { "y", DontDelete, (intptr_t)jsSVGPathSegArcRelY, (intptr_t)setJSSVGPathSegArcRelY },
    { "r1", DontDelete, (intptr_t)jsSVGPathSegArcRelR1, (intptr_t)setJSSVGPathSegArcRelR1 },
    { "r2", DontDelete, (intptr_t)jsSVGPathSegArcRelR2, (intptr_t)setJSSVGPathSegArcRelR2 },
    { "angle", DontDelete, (intptr_t)jsSVGPathSegArcRelAngle, (intptr_t)setJSSVGPathSegArcRelAngle },
    { "largeArcFlag", DontDelete, (intptr_t)jsSVGPathSegArcRelLargeArcFlag, (intptr_t)setJSSVGPathSegArcRelLargeArcFlag },
    { "sweepFlag", DontDelete, (intptr_t)jsSVGPathSegArcRelSweepFlag, (intptr_t)setJSSVGPathSegArcRelSweepFlag },
    { 0, 0, 0, 0 }
};

static const HashTable JSSVGPathSegArcRelTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 15, JSSVGPathSegArcRelTableValues, 0 };
#else
    { 16, 15, JSSVGPathSegArcRelTableValues, 0 };
#endif

/* Hash table for prototype */

static const HashTableValue JSSVGPathSegArcRelPrototypeTableValues[1] =
{
    { 0, 0, 0, 0 }
};

static const HashTable JSSVGPathSegArcRelPrototypeTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 0, JSSVGPathSegArcRelPrototypeTableValues, 0 };
#else
    { 1, 0, JSSVGPathSegArcRelPrototypeTableValues, 0 };
#endif

const ClassInfo JSSVGPathSegArcRelPrototype::s_info = { "SVGPathSegArcRelPrototype", 0, &JSSVGPathSegArcRelPrototypeTable, 0 };

JSObject* JSSVGPathSegArcRelPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSSVGPathSegArcRel>(exec, globalObject);
}

const ClassInfo JSSVGPathSegArcRel::s_info = { "SVGPathSegArcRel", &JSSVGPathSeg::s_info, &JSSVGPathSegArcRelTable, 0 };

JSSVGPathSegArcRel::JSSVGPathSegArcRel(PassRefPtr<Structure> structure, PassRefPtr<SVGPathSegArcRel> impl, SVGElement* context)
    : JSSVGPathSeg(structure, impl, context)
{
}

JSObject* JSSVGPathSegArcRel::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return new (exec) JSSVGPathSegArcRelPrototype(JSSVGPathSegArcRelPrototype::createStructure(JSSVGPathSegPrototype::self(exec, globalObject)));
}

bool JSSVGPathSegArcRel::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSSVGPathSegArcRel, Base>(exec, &JSSVGPathSegArcRelTable, this, propertyName, slot);
}

JSValuePtr jsSVGPathSegArcRelX(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    SVGPathSegArcRel* imp = static_cast<SVGPathSegArcRel*>(static_cast<JSSVGPathSegArcRel*>(asObject(slot.slotBase()))->impl());
    return jsNumber(exec, imp->x());
}

JSValuePtr jsSVGPathSegArcRelY(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    SVGPathSegArcRel* imp = static_cast<SVGPathSegArcRel*>(static_cast<JSSVGPathSegArcRel*>(asObject(slot.slotBase()))->impl());
    return jsNumber(exec, imp->y());
}

JSValuePtr jsSVGPathSegArcRelR1(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    SVGPathSegArcRel* imp = static_cast<SVGPathSegArcRel*>(static_cast<JSSVGPathSegArcRel*>(asObject(slot.slotBase()))->impl());
    return jsNumber(exec, imp->r1());
}

JSValuePtr jsSVGPathSegArcRelR2(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    SVGPathSegArcRel* imp = static_cast<SVGPathSegArcRel*>(static_cast<JSSVGPathSegArcRel*>(asObject(slot.slotBase()))->impl());
    return jsNumber(exec, imp->r2());
}

JSValuePtr jsSVGPathSegArcRelAngle(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    SVGPathSegArcRel* imp = static_cast<SVGPathSegArcRel*>(static_cast<JSSVGPathSegArcRel*>(asObject(slot.slotBase()))->impl());
    return jsNumber(exec, imp->angle());
}

JSValuePtr jsSVGPathSegArcRelLargeArcFlag(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    SVGPathSegArcRel* imp = static_cast<SVGPathSegArcRel*>(static_cast<JSSVGPathSegArcRel*>(asObject(slot.slotBase()))->impl());
    return jsBoolean(imp->largeArcFlag());
}

JSValuePtr jsSVGPathSegArcRelSweepFlag(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    SVGPathSegArcRel* imp = static_cast<SVGPathSegArcRel*>(static_cast<JSSVGPathSegArcRel*>(asObject(slot.slotBase()))->impl());
    return jsBoolean(imp->sweepFlag());
}

void JSSVGPathSegArcRel::put(ExecState* exec, const Identifier& propertyName, JSValuePtr value, PutPropertySlot& slot)
{
    lookupPut<JSSVGPathSegArcRel, Base>(exec, propertyName, value, &JSSVGPathSegArcRelTable, this, slot);
}

void setJSSVGPathSegArcRelX(ExecState* exec, JSObject* thisObject, JSValuePtr value)
{
    SVGPathSegArcRel* imp = static_cast<SVGPathSegArcRel*>(static_cast<JSSVGPathSegArcRel*>(thisObject)->impl());
    imp->setX(value->toFloat(exec));
    if (static_cast<JSSVGPathSegArcRel*>(thisObject)->context())
        static_cast<JSSVGPathSegArcRel*>(thisObject)->context()->svgAttributeChanged(static_cast<JSSVGPathSegArcRel*>(thisObject)->impl()->associatedAttributeName());
}

void setJSSVGPathSegArcRelY(ExecState* exec, JSObject* thisObject, JSValuePtr value)
{
    SVGPathSegArcRel* imp = static_cast<SVGPathSegArcRel*>(static_cast<JSSVGPathSegArcRel*>(thisObject)->impl());
    imp->setY(value->toFloat(exec));
    if (static_cast<JSSVGPathSegArcRel*>(thisObject)->context())
        static_cast<JSSVGPathSegArcRel*>(thisObject)->context()->svgAttributeChanged(static_cast<JSSVGPathSegArcRel*>(thisObject)->impl()->associatedAttributeName());
}

void setJSSVGPathSegArcRelR1(ExecState* exec, JSObject* thisObject, JSValuePtr value)
{
    SVGPathSegArcRel* imp = static_cast<SVGPathSegArcRel*>(static_cast<JSSVGPathSegArcRel*>(thisObject)->impl());
    imp->setR1(value->toFloat(exec));
    if (static_cast<JSSVGPathSegArcRel*>(thisObject)->context())
        static_cast<JSSVGPathSegArcRel*>(thisObject)->context()->svgAttributeChanged(static_cast<JSSVGPathSegArcRel*>(thisObject)->impl()->associatedAttributeName());
}

void setJSSVGPathSegArcRelR2(ExecState* exec, JSObject* thisObject, JSValuePtr value)
{
    SVGPathSegArcRel* imp = static_cast<SVGPathSegArcRel*>(static_cast<JSSVGPathSegArcRel*>(thisObject)->impl());
    imp->setR2(value->toFloat(exec));
    if (static_cast<JSSVGPathSegArcRel*>(thisObject)->context())
        static_cast<JSSVGPathSegArcRel*>(thisObject)->context()->svgAttributeChanged(static_cast<JSSVGPathSegArcRel*>(thisObject)->impl()->associatedAttributeName());
}

void setJSSVGPathSegArcRelAngle(ExecState* exec, JSObject* thisObject, JSValuePtr value)
{
    SVGPathSegArcRel* imp = static_cast<SVGPathSegArcRel*>(static_cast<JSSVGPathSegArcRel*>(thisObject)->impl());
    imp->setAngle(value->toFloat(exec));
    if (static_cast<JSSVGPathSegArcRel*>(thisObject)->context())
        static_cast<JSSVGPathSegArcRel*>(thisObject)->context()->svgAttributeChanged(static_cast<JSSVGPathSegArcRel*>(thisObject)->impl()->associatedAttributeName());
}

void setJSSVGPathSegArcRelLargeArcFlag(ExecState* exec, JSObject* thisObject, JSValuePtr value)
{
    SVGPathSegArcRel* imp = static_cast<SVGPathSegArcRel*>(static_cast<JSSVGPathSegArcRel*>(thisObject)->impl());
    imp->setLargeArcFlag(value->toBoolean(exec));
    if (static_cast<JSSVGPathSegArcRel*>(thisObject)->context())
        static_cast<JSSVGPathSegArcRel*>(thisObject)->context()->svgAttributeChanged(static_cast<JSSVGPathSegArcRel*>(thisObject)->impl()->associatedAttributeName());
}

void setJSSVGPathSegArcRelSweepFlag(ExecState* exec, JSObject* thisObject, JSValuePtr value)
{
    SVGPathSegArcRel* imp = static_cast<SVGPathSegArcRel*>(static_cast<JSSVGPathSegArcRel*>(thisObject)->impl());
    imp->setSweepFlag(value->toBoolean(exec));
    if (static_cast<JSSVGPathSegArcRel*>(thisObject)->context())
        static_cast<JSSVGPathSegArcRel*>(thisObject)->context()->svgAttributeChanged(static_cast<JSSVGPathSegArcRel*>(thisObject)->impl()->associatedAttributeName());
}


}

#endif // ENABLE(SVG)