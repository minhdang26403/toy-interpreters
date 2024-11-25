#ifndef AST_PRINTER_H_
#define AST_PRINTER_H_

#include <format>
#include "expr.h"

namespace cclox {
/**
 * @brief A visitor class that converts Abstract Syntax Tree (AST) nodes to
 * their string representation
 *
 * This class implements the visitor pattern using std::variant to traverse and
 * print different types of expressions in the AST. Each operator() overload
 * handles a specific type of expression node and formats it according to a
 * Lisp-like prefix notation.
 *
 * Example output formats:
 * - Binary: "(+ 1 2)"
 * - Grouping: "(group 1)"
 * - Unary: "(- 1)"
 * - Literal: "123" or "hello"
 */
class ASTPrinter {
 public:
  auto Print(const ExprPtr& expr) -> std::string {
    return std::visit(*this, expr);
  }

  /**
   * @brief Formats a binary expression node
   * @param expr Shared pointer to the binary expression
   * @return String representation in the format "(operator left_expr
   * right_expr)"
   */
  auto operator()(const BinaryPtr& expr) -> std::string {
    return Parenthesize(expr->GetOperator().GetLexeme(), expr->GetLeftExpr(),
                        expr->GetRightExpr());
  }

  /**
   * @brief Formats a grouping expression node
   * @param expr Shared pointer to the grouping expression
   * @return String representation in the format "(group expr)"
   */
  auto operator()(const GroupingPtr& expr) -> std::string {
    return Parenthesize("group", expr->GetExpr());
  }

  /**
   * @brief Formats a literal expression node
   * @param expr Shared pointer to the literal expression
   * @return String representation of the literal value
   */
  auto operator()(const LiteralPtr& expr) -> std::string {
    return expr->GetValue().ToString();
  }

  /**
   * @brief Formats a unary expression node
   * @param expr Shared pointer to the unary expression
   * @return String representation in the format "(operator expr)"
   */
  auto operator()(const UnaryPtr& expr) -> std::string {
    return Parenthesize(expr->GetOperator().GetLexeme(),
                        expr->GetRightExpression());
  }

 private:
  /**
   * @brief Helper function to format an expression with one operand
   * @param name The operator or node type name
   * @param expr The operand expression
   * @return Formatted string with parentheses: "(name expr)"
   */
  auto Parenthesize(std::string_view name, const ExprPtr& expr) -> std::string {
    return std::format("({} {})", name, Print(expr));
  }

  /**
   * @brief Helper function to format an expression with two operands
   * @param name The operator or node type name
   * @param expr1 The first operand expression
   * @param expr2 The second operand expression
   * @return Formatted string with parentheses: "(name expr1 expr2)"
   */
  auto Parenthesize(std::string_view name, const ExprPtr& expr1,
                    const ExprPtr& expr2) -> std::string {
    return std::format("({} {} {})", name, Print(expr1), Print(expr2));
  }
};
}  // namespace cclox

#endif  // AST_PRINTER_H_