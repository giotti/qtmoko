// This file was generated by qlalr - DO NOT EDIT!

/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtScript module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at http://www.qtsoftware.com/contact.
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtCore/QtDebug>

#ifndef QT_NO_SCRIPT

#include <string.h>

#include "qscriptengine.h"
#include "qscriptengine_p.h"
#include "qscriptvalueimpl_p.h"
#include "qscriptcontext_p.h"
#include "qscriptmember_p.h"
#include "qscriptobject_p.h"
#include "qscriptlexer_p.h"
#include "qscriptast_p.h"
#include "qscriptnodepool_p.h"

#define Q_SCRIPT_UPDATE_POSITION(node, startloc, endloc) do { \
    node->startLine = startloc.startLine; \
    node->startColumn = startloc.startColumn; \
    node->endLine = endloc.endLine; \
    node->endColumn = endloc.endColumn; \
} while (0)



#include "qscriptparser_p.h"

//
// This file is automatically generated from qscript.g.
// Changes will be lost.
//

QT_BEGIN_NAMESPACE

inline static bool automatic(QScriptEnginePrivate *driver, int token)
{
    return token == QScriptGrammar::T_RBRACE
        || token == 0
        || driver->lexer()->prevTerminator();
}


QScriptParser::QScriptParser():
    tos(0),
    stack_size(0),
    sym_stack(0),
    state_stack(0),
    location_stack(0),
    error_lineno(0),
    error_column(0)
{
}

QScriptParser::~QScriptParser()
{
    if (stack_size) {
        qFree(sym_stack);
        qFree(state_stack);
        qFree(location_stack);
    }
}

static inline QScriptParser::Location location(QScript::Lexer *lexer)
{
    QScriptParser::Location loc;
    loc.startLine = lexer->startLineNo();
    loc.startColumn = lexer->startColumnNo();
    loc.endLine = lexer->endLineNo();
    loc.endColumn = lexer->endColumnNo();
    return loc;
}

bool QScriptParser::parse(QScriptEnginePrivate *driver)
{
  const int INITIAL_STATE = 0;
  QScript::Lexer *lexer = driver->lexer();

  int yytoken = -1;
  int saved_yytoken = -1;

  reallocateStack();

  tos = 0;
  state_stack[++tos] = INITIAL_STATE;

  while (true)
    {
      const int state = state_stack [tos];
      if (yytoken == -1 && - TERMINAL_COUNT != action_index [state])
        {
          if (saved_yytoken == -1)
            {
              yytoken = lexer->lex();
              location_stack [tos] = location(lexer);
            }
          else
            {
              yytoken = saved_yytoken;
              saved_yytoken = -1;
            }
        }

      int act = t_action (state, yytoken);

      if (act == ACCEPT_STATE)
        return true;

      else if (act > 0)
        {
          if (++tos == stack_size)
            reallocateStack();

          sym_stack [tos].dval = lexer->dval ();
          state_stack [tos] = act;
          location_stack [tos] = location(lexer);
          yytoken = -1;
        }

      else if (act < 0)
        {
          int r = - act - 1;

          tos -= rhs [r];
          act = state_stack [tos++];

          switch (r) {

case 0: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::ThisExpression> (driver->nodePool());
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(1));
} break;

case 1: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::IdentifierExpression> (driver->nodePool(), sym(1).sval);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(1));
} break;

case 2: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::NullExpression> (driver->nodePool());
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(1));
} break;

case 3: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::TrueLiteral> (driver->nodePool());
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(1));
} break;

case 4: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::FalseLiteral> (driver->nodePool());
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(1));
} break;

case 5: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::NumericLiteral> (driver->nodePool(), sym(1).dval);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(1));
} break;

case 6: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::StringLiteral> (driver->nodePool(), sym(1).sval);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(1));
} break;

case 7: {
  bool rx = lexer->scanRegExp(QScript::Lexer::NoPrefix);
  if (!rx) {
      error_message = lexer->errorMessage();
      error_lineno = lexer->startLineNo();
      error_column = lexer->startColumnNo();
      return false;
  }
  sym(1).Node = QScript::makeAstNode<QScript::AST::RegExpLiteral> (driver->nodePool(), lexer->pattern, lexer->flags);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(1));
} break;

case 8: {
  bool rx = lexer->scanRegExp(QScript::Lexer::EqualPrefix);
  if (!rx) {
      error_message = lexer->errorMessage();
      error_lineno = lexer->startLineNo();
      error_column = lexer->startColumnNo();
      return false;
  }
  sym(1).Node = QScript::makeAstNode<QScript::AST::RegExpLiteral> (driver->nodePool(), lexer->pattern, lexer->flags);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(1));
} break;

case 9: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::ArrayLiteral> (driver->nodePool(), sym(2).Elision);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 10: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::ArrayLiteral> (driver->nodePool(), sym(2).ElementList->finish ());
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 11: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::ArrayLiteral> (driver->nodePool(), sym(2).ElementList->finish (), sym(4).Elision);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(5));
} break;

case 12: {
  if (sym(2).Node)
    sym(1).Node = QScript::makeAstNode<QScript::AST::ObjectLiteral> (driver->nodePool(), sym(2).PropertyNameAndValueList->finish ());
  else
    sym(1).Node = QScript::makeAstNode<QScript::AST::ObjectLiteral> (driver->nodePool());
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 13: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::ObjectLiteral> (driver->nodePool(), sym(2).PropertyNameAndValueList->finish ());
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(4));
} break;

case 14: {
  sym(1) = sym(2);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 15: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::ElementList> (driver->nodePool(), sym(1).Elision, sym(2).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(2));
} break;

case 16: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::ElementList> (driver->nodePool(), sym(1).ElementList, sym(3).Elision, sym(4).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(4));
} break;

case 17: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::Elision> (driver->nodePool());
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(1));
} break;

case 18: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::Elision> (driver->nodePool(), sym(1).Elision);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(2));
} break;

case 19: {
  sym(1).Node = 0;
} break;

case 20: {
  sym(1).Elision = sym(1).Elision->finish ();
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(1));
} break;

case 21: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::PropertyNameAndValueList> (driver->nodePool(), sym(1).PropertyName, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 22: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::PropertyNameAndValueList> (driver->nodePool(), sym(1).PropertyNameAndValueList, sym(3).PropertyName, sym(5).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(5));
} break;

case 23: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::IdentifierPropertyName> (driver->nodePool(), sym(1).sval);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(1));
} break;

case 24: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::StringLiteralPropertyName> (driver->nodePool(), sym(1).sval);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(1));
} break;

case 25: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::NumericLiteralPropertyName> (driver->nodePool(), sym(1).dval);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(1));
} break;

case 26: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::IdentifierPropertyName> (driver->nodePool(), sym(1).sval);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(1));
} break;

case 27:

case 28:

case 29:

case 30:

case 31:

case 32:

case 33:

case 34:

case 35:

case 36:

case 37:

case 38:

case 39:

case 40:

case 41:

case 42:

case 43:

case 44:

case 45:

case 46:

case 47:

case 48:

case 49:

case 50:

case 51:

case 52:

case 53:

case 54:

case 55:

case 56:

case 57:
{
  sym(1).sval = driver->intern(lexer->characterBuffer(), lexer->characterCount());
} break;

case 62: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::ArrayMemberExpression> (driver->nodePool(), sym(1).Expression, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(4));
} break;

case 63: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::FieldMemberExpression> (driver->nodePool(), sym(1).Expression, sym(3).sval);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(4));
} break;

case 64: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::NewMemberExpression> (driver->nodePool(), sym(2).Expression, sym(3).ArgumentList);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 66: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::NewExpression> (driver->nodePool(), sym(2).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(2));
} break;

case 67: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::CallExpression> (driver->nodePool(), sym(1).Expression, sym(2).ArgumentList);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(2));
} break;

case 68: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::CallExpression> (driver->nodePool(), sym(1).Expression, sym(2).ArgumentList);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(2));
} break;

case 69: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::ArrayMemberExpression> (driver->nodePool(), sym(1).Expression, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(4));
} break;

case 70: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::FieldMemberExpression> (driver->nodePool(), sym(1).Expression, sym(3).sval);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 71: {
  sym(1).Node = 0;
} break;

case 72: {
  sym(1).Node = sym(2).ArgumentList->finish ();
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 73: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::ArgumentList> (driver->nodePool(), sym(1).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(1));
} break;

case 74: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::ArgumentList> (driver->nodePool(), sym(1).ArgumentList, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 78: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::PostIncrementExpression> (driver->nodePool(), sym(1).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(2));
} break;

case 79: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::PostDecrementExpression> (driver->nodePool(), sym(1).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(2));
} break;

case 81: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::DeleteExpression> (driver->nodePool(), sym(2).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(2));
} break;

case 82: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::VoidExpression> (driver->nodePool(), sym(2).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(2));
} break;

case 83: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::TypeOfExpression> (driver->nodePool(), sym(2).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(2));
} break;

case 84: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::PreIncrementExpression> (driver->nodePool(), sym(2).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(2));
} break;

case 85: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::PreDecrementExpression> (driver->nodePool(), sym(2).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(2));
} break;

case 86: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::UnaryPlusExpression> (driver->nodePool(), sym(2).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(2));
} break;

case 87: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::UnaryMinusExpression> (driver->nodePool(), sym(2).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(2));
} break;

case 88: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::TildeExpression> (driver->nodePool(), sym(2).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(2));
} break;

case 89: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::NotExpression> (driver->nodePool(), sym(2).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(2));
} break;

case 91: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::Mul, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 92: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::Div, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 93: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::Mod, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 95: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::Add, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 96: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::Sub, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 98: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::LShift, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 99: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::RShift, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 100: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::URShift, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 102: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::Lt, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 103: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::Gt, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 104: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::Le, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 105: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::Ge, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 106: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::InstanceOf, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 107: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::In, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 109: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::Lt, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 110: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::Gt, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 111: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::Le, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 112: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::Ge, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 113: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::InstanceOf, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 115: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::Equal, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 116: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::NotEqual, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 117: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::StrictEqual, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 118: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::StrictNotEqual, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 120: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::Equal, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 121: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::NotEqual, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 122: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::StrictEqual, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 123: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::StrictNotEqual, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 125: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::BitAnd, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 127: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::BitAnd, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 129: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::BitXor, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 131: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::BitXor, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 133: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::BitOr, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 135: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::BitOr, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 137: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::And, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 139: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::And, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 141: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::Or, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 143: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, QSOperator::Or, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 145: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::ConditionalExpression> (driver->nodePool(), sym(1).Expression, sym(3).Expression, sym(5).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 147: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::ConditionalExpression> (driver->nodePool(), sym(1).Expression, sym(3).Expression, sym(5).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 149: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, sym(2).ival, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 151: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BinaryExpression> (driver->nodePool(), sym(1).Expression, sym(2).ival, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 152: {
  sym(1).ival = QSOperator::Assign;
} break;

case 153: {
  sym(1).ival = QSOperator::InplaceMul;
} break;

case 154: {
  sym(1).ival = QSOperator::InplaceDiv;
} break;

case 155: {
  sym(1).ival = QSOperator::InplaceMod;
} break;

case 156: {
  sym(1).ival = QSOperator::InplaceAdd;
} break;

case 157: {
  sym(1).ival = QSOperator::InplaceSub;
} break;

case 158: {
  sym(1).ival = QSOperator::InplaceLeftShift;
} break;

case 159: {
  sym(1).ival = QSOperator::InplaceRightShift;
} break;

case 160: {
  sym(1).ival = QSOperator::InplaceURightShift;
} break;

case 161: {
  sym(1).ival = QSOperator::InplaceAnd;
} break;

case 162: {
  sym(1).ival = QSOperator::InplaceXor;
} break;

case 163: {
  sym(1).ival = QSOperator::InplaceOr;
} break;

case 165: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::Expression> (driver->nodePool(), sym(1).Expression, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 166: {
  sym(1).Node = 0;
} break;

case 169: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::Expression> (driver->nodePool(), sym(1).Expression, sym(3).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 170: {
  sym(1).Node = 0;
} break;

case 187: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::Block> (driver->nodePool(), sym(2).StatementList);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 188: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::StatementList> (driver->nodePool(), sym(1).Statement);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(1));
} break;

case 189: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::StatementList> (driver->nodePool(), sym(1).StatementList, sym(2).Statement);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(2));
} break;

case 190: {
  sym(1).Node = 0;
} break;

case 191: {
  sym(1).Node = sym(1).StatementList->finish ();
} break;

case 193: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::VariableStatement> (driver->nodePool(), sym(2).VariableDeclarationList->finish (/*readOnly=*/sym(1).ival == T_CONST));
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 194: {
  sym(1).ival = T_CONST;
} break;

case 195: {
  sym(1).ival = T_VAR;
} break;

case 196: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::VariableDeclarationList> (driver->nodePool(), sym(1).VariableDeclaration);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(1));
} break;

case 197: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::VariableDeclarationList> (driver->nodePool(), sym(1).VariableDeclarationList, sym(3).VariableDeclaration);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 198: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::VariableDeclarationList> (driver->nodePool(), sym(1).VariableDeclaration);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(1));
} break;

case 199: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::VariableDeclarationList> (driver->nodePool(), sym(1).VariableDeclarationList, sym(3).VariableDeclaration);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 200: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::VariableDeclaration> (driver->nodePool(), sym(1).sval, sym(2).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(2));
} break;

case 201: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::VariableDeclaration> (driver->nodePool(), sym(1).sval, sym(2).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(2));
} break;

case 202: {
  sym(1) = sym(2);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(2));
} break;

case 203: {
  sym(1).Node = 0;
} break;

case 205: {
  sym(1) = sym(2);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(2));
} break;

case 206: {
  sym(1).Node = 0;
} break;

case 208: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::EmptyStatement> (driver->nodePool());
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(1));
} break;

case 210: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::ExpressionStatement> (driver->nodePool(), sym(1).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(2));
} break;

case 211: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::IfStatement> (driver->nodePool(), sym(3).Expression, sym(5).Statement, sym(7).Statement);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(7));
} break;

case 212: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::IfStatement> (driver->nodePool(), sym(3).Expression, sym(5).Statement);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(5));
} break;

case 214: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::DoWhileStatement> (driver->nodePool(), sym(2).Statement, sym(5).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(7));
} break;

case 215: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::WhileStatement> (driver->nodePool(), sym(3).Expression, sym(5).Statement);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(5));
} break;

case 216: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::ForStatement> (driver->nodePool(), sym(3).Expression, sym(5).Expression, sym(7).Expression, sym(9).Statement);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(9));
} break;

case 217: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::LocalForStatement> (driver->nodePool(), sym(4).VariableDeclarationList->finish (/*readOnly=*/false), sym(6).Expression, sym(8).Expression, sym(10).Statement);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(10));
} break;

case 218: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::ForEachStatement> (driver->nodePool(), sym(3).Expression, sym(5).Expression, sym(7).Statement);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(7));
} break;

case 219: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::LocalForEachStatement> (driver->nodePool(), sym(4).VariableDeclaration, sym(6).Expression, sym(8).Statement);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(8));
} break;

case 221: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::ContinueStatement> (driver->nodePool());
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(2));
} break;

case 223: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::ContinueStatement> (driver->nodePool(), sym(2).sval);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 225: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BreakStatement> (driver->nodePool());
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(2));
} break;

case 227: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::BreakStatement> (driver->nodePool(), sym(2).sval);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 229: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::ReturnStatement> (driver->nodePool(), sym(2).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 230: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::WithStatement> (driver->nodePool(), sym(3).Expression, sym(5).Statement);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(5));
} break;

case 231: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::SwitchStatement> (driver->nodePool(), sym(3).Expression, sym(5).CaseBlock);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(5));
} break;

case 232: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::CaseBlock> (driver->nodePool(), sym(2).CaseClauses);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 233: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::CaseBlock> (driver->nodePool(), sym(2).CaseClauses, sym(3).DefaultClause, sym(4).CaseClauses);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(5));
} break;

case 234: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::CaseClauses> (driver->nodePool(), sym(1).CaseClause);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(1));
} break;

case 235: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::CaseClauses> (driver->nodePool(), sym(1).CaseClauses, sym(2).CaseClause);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(2));
} break;

case 236: {
  sym(1).Node = 0;
} break;

case 237: {
  sym(1).Node = sym(1).CaseClauses->finish ();
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(1));
} break;

case 238: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::CaseClause> (driver->nodePool(), sym(2).Expression, sym(4).StatementList);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(4));
} break;

case 239: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::DefaultClause> (driver->nodePool(), sym(3).StatementList);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 240: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::LabelledStatement> (driver->nodePool(), sym(1).sval, sym(3).Statement);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 242: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::ThrowStatement> (driver->nodePool(), sym(2).Expression);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 243: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::TryStatement> (driver->nodePool(), sym(2).Statement, sym(3).Catch);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 244: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::TryStatement> (driver->nodePool(), sym(2).Statement, sym(3).Finally);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 245: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::TryStatement> (driver->nodePool(), sym(2).Statement, sym(3).Catch, sym(4).Finally);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(4));
} break;

case 246: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::Catch> (driver->nodePool(), sym(3).sval, sym(5).Statement);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(5));
} break;

case 247: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::Finally> (driver->nodePool(), sym(2).Statement);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(2));
} break;

case 249: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::DebuggerStatement> (driver->nodePool());
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(1));
} break;

case 250: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::FunctionDeclaration> (driver->nodePool(), sym(2).sval, sym(4).FormalParameterList, sym(7).FunctionBody);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(8));
} break;

case 251: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::FunctionExpression> (driver->nodePool(), sym(2).sval, sym(4).FormalParameterList, sym(7).FunctionBody);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(8));
} break;

case 252: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::FormalParameterList> (driver->nodePool(), sym(1).sval);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(1));
} break;

case 253: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::FormalParameterList> (driver->nodePool(), sym(1).FormalParameterList, sym(3).sval);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(3));
} break;

case 254: {
  sym(1).Node = 0;
} break;

case 255: {
  sym(1).Node = sym(1).FormalParameterList->finish ();
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(1));
} break;

case 256: {
  sym(1).Node = 0;
} break;

case 258: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::FunctionBody> (driver->nodePool(), sym(1).SourceElements->finish ());
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(1));
} break;

case 259: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::Program> (driver->nodePool(), sym(1).SourceElements->finish ());
  driver->changeAbstractSyntaxTree(sym(1).Node);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(1));
} break;

case 260: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::SourceElements> (driver->nodePool(), sym(1).SourceElement);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(1));
} break;

case 261: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::SourceElements> (driver->nodePool(), sym(1).SourceElements, sym(2).SourceElement);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(2));
} break;

case 262: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::StatementSourceElement> (driver->nodePool(), sym(1).Statement);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(1));
} break;

case 263: {
  sym(1).Node = QScript::makeAstNode<QScript::AST::FunctionSourceElement> (driver->nodePool(), sym(1).FunctionDeclaration);
  Q_SCRIPT_UPDATE_POSITION(sym(1).Node, loc(1), loc(1));
} break;

case 264: {
  sym(1).sval = 0;
} break;

case 266: {
  sym(1).Node = 0;
} break;

          } // switch

          state_stack [tos] = nt_action (act, lhs [r] - TERMINAL_COUNT);

          if (rhs[r] > 1) {
              location_stack[tos - 1].endLine = location_stack[tos + rhs[r] - 2].endLine;
              location_stack[tos - 1].endColumn = location_stack[tos + rhs[r] - 2].endColumn;
              location_stack[tos] = location_stack[tos + rhs[r] - 1];
          }
        }

      else
        {
          if (saved_yytoken == -1 && automatic (driver, yytoken) && t_action (state, T_AUTOMATIC_SEMICOLON) > 0)
            {
              saved_yytoken = yytoken;
              yytoken = T_SEMICOLON;
              continue;
            }

          else if ((state == INITIAL_STATE) && (yytoken == 0)) {
              // accept empty input
              yytoken = T_SEMICOLON;
              continue;
          }

          int ers = state;
          int shifts = 0;
          int reduces = 0;
          int expected_tokens [3];
          for (int tk = 0; tk < TERMINAL_COUNT; ++tk)
            {
              int k = t_action (ers, tk);

              if (! k)
                continue;
              else if (k < 0)
                ++reduces;
              else if (spell [tk])
                {
                  if (shifts < 3)
                    expected_tokens [shifts] = tk;
                  ++shifts;
                }
            }

          error_message.clear ();
          if (shifts && shifts < 3)
            {
              bool first = true;

              for (int s = 0; s < shifts; ++s)
                {
                  if (first)
                    error_message += QLatin1String ("Expected ");
                  else
                    error_message += QLatin1String (", ");

                  first = false;
                  error_message += QLatin1String("`");
                  error_message += QLatin1String (spell [expected_tokens [s]]);
                  error_message += QLatin1String("'");
                }
            }

          if (error_message.isEmpty())
              error_message = lexer->errorMessage();

          error_lineno = lexer->startLineNo();
          error_column = lexer->startColumnNo();

          return false;
        }
    }

    return false;
}

QT_END_NAMESPACE

#endif // QT_NO_SCRIPT