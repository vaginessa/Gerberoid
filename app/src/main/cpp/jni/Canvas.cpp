/*
 * This file is part of the Gerberoid project.
 *
 * Copyright (C) 2017 Marcus Comstedt <marcus@mc.pp.se>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <jni.h>
#include "Canvas.h"
#include "Bitmap.h"
#include "Paint.h"
#include "inithook.h"
#include "localframe.h"

namespace android
{

class Canvas::Native : InitHook
{
  friend class Canvas;
  virtual bool init(JNIEnv *env);
  virtual void deinit(JNIEnv *env);
private:
  static Native hook;

  static jclass class_Canvas;
  static jmethodID method_init;
  static jmethodID method_setBitmap;
  static jmethodID method_save;
  static jmethodID method_restore;
  static jmethodID method_scale;
  static jmethodID method_translate;
  static jmethodID method_clipRect;
  static jmethodID method_drawCircle;

  static JNIRef createCanvas();
};
Canvas::Native Canvas::Native::hook;

jclass Canvas::Native::class_Canvas = 0;
jmethodID Canvas::Native::method_init = 0;
jmethodID Canvas::Native::method_setBitmap = 0;
jmethodID Canvas::Native::method_save = 0;
jmethodID Canvas::Native::method_restore = 0;
jmethodID Canvas::Native::method_scale = 0;
jmethodID Canvas::Native::method_translate = 0;
jmethodID Canvas::Native::method_clipRect = 0;
jmethodID Canvas::Native::method_drawCircle = 0;

Canvas::Canvas() : JNIRef(Native::createCanvas()) {}

void Canvas::setBitmap(Bitmap &bitmap)
{
  LocalFrame env;
  if (!env || !get() || !bitmap) return;
  env->CallVoidMethod(*this, Native::method_setBitmap,
		      static_cast<jobject>(bitmap));
}

int Canvas::save()
{
  LocalFrame env;
  if (!env || !get()) return 0;
  return env->CallIntMethod(*this, Native::method_save);
}

void Canvas::restore()
{
  LocalFrame env;
  if (!env || !get()) return;
  env->CallVoidMethod(*this, Native::method_restore);
}

void Canvas::scale(float sx, float sy)
{
  LocalFrame env;
  if (!env || !get()) return;
  env->CallVoidMethod(*this, Native::method_scale,
		      static_cast<jfloat>(sx), static_cast<jfloat>(sy));
}

void Canvas::translate(float dx, float dy)
{
  LocalFrame env;
  if (!env || !get()) return;
  env->CallVoidMethod(*this, Native::method_translate,
		      static_cast<jfloat>(dx), static_cast<jfloat>(dy));
}

bool Canvas::clipRect(int left, int top, int right, int bottom)
{
  LocalFrame env;
  if (!env || !get()) return false;
  return env->CallBooleanMethod(*this, Native::method_clipRect,
				static_cast<jint>(left),
				static_cast<jint>(top),
				static_cast<jint>(right),
				static_cast<jint>(bottom));
}

void Canvas::drawCircle(float cx, float cy, float radius, const Paint& paint)
{
  LocalFrame env;
  if (!env || !get() || !paint) return;
  env->CallVoidMethod(*this, Native::method_drawCircle,
				static_cast<jfloat>(cx),
				static_cast<jfloat>(cy),
				static_cast<jfloat>(radius),
				static_cast<jobject>(paint));
}

JNIRef Canvas::Native::createCanvas()
{
  LocalFrame env;
  if (!env) return JNIRef();
  return env->NewObject(class_Canvas, method_init);
}

bool Canvas::Native::init(JNIEnv *env)
{
  jclass loc_class = env->FindClass("android/graphics/Canvas");
  if (!loc_class)
    return false;
  class_Canvas = (jclass)env->NewGlobalRef(loc_class);
  env->DeleteLocalRef(loc_class);
  if (!class_Canvas)
    return false;
  method_init = env->GetMethodID(class_Canvas, "<init>", "()V");
  method_setBitmap = env->GetMethodID(class_Canvas, "setBitmap", "(Landroid/graphics/Bitmap;)V");
  method_save = env->GetMethodID(class_Canvas, "save", "()I");
  method_restore = env->GetMethodID(class_Canvas, "restore", "()V");
  method_scale = env->GetMethodID(class_Canvas, "scale", "(FF)V");
  method_translate = env->GetMethodID(class_Canvas, "translate", "(FF)V");
  method_clipRect = env->GetMethodID(class_Canvas, "clipRect", "(IIII)Z");
  method_drawCircle = env->GetMethodID(class_Canvas, "drawCircle", "(FFFLandroid/graphics/Paint;)V");
  return method_init && method_setBitmap && method_save && method_restore &&
    method_scale && method_translate && method_clipRect && method_drawCircle;
}

void Canvas::Native::deinit(JNIEnv *env)
{
  method_init = 0;
  method_setBitmap = 0;
  method_save = 0;
  method_restore = 0;
  method_scale = 0;
  method_translate = 0;
  method_clipRect = 0;
  method_drawCircle = 0;
  if (class_Canvas) {
    env->DeleteGlobalRef(class_Canvas);
    class_Canvas = 0;
  }
}

}
