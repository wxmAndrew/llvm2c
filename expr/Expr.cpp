#include "Expr.h"
#include "UnaryExpr.h"

#include "llvm/Support/raw_ostream.h"

Struct::Struct(const std::string& name)
    : name(name) {
    setType(std::make_unique<StructType>(this->name));
}

void Struct::addItem(std::unique_ptr<Type> type, const std::string& name) {
    items.push_back(std::make_pair(std::move(type), name));
}

void Struct::accept(ExprVisitor& visitor) {
    visitor.visit(*this);
}

StructElement::StructElement(Struct* strct, Expr* expr, unsigned element)
    : strct(strct),
      expr(expr),
      element(element) {
    setType(strct->items[element].first->clone());
}

void StructElement::accept(ExprVisitor& visitor) {
    visitor.visit(*this);
}

ArrayElement::ArrayElement(Expr* expr, Expr* elem)
    : expr(expr),
      element(elem) {
    ArrayType* AT = static_cast<ArrayType*>(expr->getType());
    setType(AT->type->clone());
}

ArrayElement::ArrayElement(Expr* expr, Expr* elem, std::unique_ptr<Type> type)
    : expr(expr),
      element(elem) {
    setType(std::move(type));
}

void ArrayElement::accept(ExprVisitor& visitor) {
    visitor.visit(*this);
}

ExtractValueExpr::ExtractValueExpr(std::vector<std::unique_ptr<Expr>>& indices) {
    for (auto& idx : indices) {
        this->indices.push_back(std::move(idx));
    }

    setType(this->indices[this->indices.size() - 1]->getType()->clone());
}

void ExtractValueExpr::accept(ExprVisitor& visitor) {
    visitor.visit(*this);
}

Value::Value(const std::string& valueName, std::unique_ptr<Type> type) {
    setType(std::move(type));
    this->valueName = valueName;
}

/* static std::string valueInitToString() { */
/*     if (!init) { */
/*         std::string ret; */
/*         if (auto PT = dynamic_cast<const PointerType*>(getType())) { */
/*             if (PT->isArrayPointer && valueName.compare("0") != 0) { */
/*                 ret = "("; */
/*                 for (unsigned i = 0; i < PT->levels; i++) { */
/*                     ret += "*"; */
/*                 } */
/*                 ret += valueName + ")"; */
/*             } */

/*             if (PT->isArrayPointer) { */
/*                 ret = ret + PT->sizes; */
/*             } */

/*             if (!ret.empty()) { */
/*                 return ret; */
/*             } */
/*         } */

/*         if (auto AT = dynamic_cast<const ArrayType*>(getType())) { */
/*             if (AT->isPointerArray && AT->pointer->isArrayPointer) { */
/*                 ret = "("; */
/*                 for (unsigned i = 0; i < AT->pointer->levels; i++) { */
/*                     ret += "*"; */
/*                 } */
/*                 return ret + valueName + AT->sizeToString() + ")" + AT->pointer->sizes; */
/*             } else { */
/*                 return valueName + AT->sizeToString(); */
/*             } */
/*         } */
/*     } */
/* } */

void Value::accept(ExprVisitor& visitor) {
    visitor.visit(*this);
}

bool Value::isZero() const {
    return valueName == "0";
}

GlobalValue::GlobalValue(const std::string& varName, const std::string& value, std::unique_ptr<Type> type)
    : Value(varName, std::move(type)),
      value(value) { }

/* static std::string globalValueToString() { */   
/*     if (!init) { */
/*         std::string ret = getType()->toString() + " "; */
/*         if (auto AT = dynamic_cast<const ArrayType*>(getType())) { */
/*             if (AT->isPointerArray && AT->pointer->isArrayPointer) { */
/*                 ret += "("; */
/*                 for (unsigned i = 0; i < AT->pointer->levels; i++) { */
/*                     ret += "*"; */
/*                 } */
/*                 ret += valueName + AT->sizeToString() + ")" + AT->pointer->sizes; */
/*             } else { */
/*                 ret += " " + valueName + AT->sizeToString();; */
/*             } */
/*         } else if (auto PT = dynamic_cast<const PointerType*>(getType())) { */
/*             if (PT->isArrayPointer && valueName.compare("0") != 0) { */
/*                 ret += "("; */
/*                 for (unsigned i = 0; i < PT->levels; i++) { */
/*                     ret += "*"; */
/*                 } */
/*                 ret += valueName + ")"; */
/*             } else { */
/*                 ret += " " + valueName; */

/*                 if (!value.empty()) { */
/*                     ret += " = " + value; */
/*                 } */

/*                 return ret + ";"; */
/*             } */

/*             if (PT->isArrayPointer) { */
/*                 ret = ret + PT->sizes; */
/*             } */
/*         } else { */
/*             ret += valueName; */
/*         } */

/*         if (!value.empty()) { */
/*             ret += " = " + value; */
/*         } */

/*         return ret + ";"; */
/*     } */
/* } */

/* std::string GlobalValue::declToString() const { */
/*     std::string ret = getType()->toString(); */
/*     if (auto AT = dynamic_cast<const ArrayType*>(getType())) { */
/*         if (AT->isPointerArray && AT->pointer->isArrayPointer) { */
/*             ret += " ("; */
/*             for (unsigned i = 0; i < AT->pointer->levels; i++) { */
/*                 ret += "*"; */
/*             } */
/*             ret += valueName + AT->sizeToString() + ")" + AT->pointer->sizes; */
/*         } else { */
/*             ret += " " + valueName + AT->sizeToString();; */
/*         } */
/*     } else if (auto PT = dynamic_cast<const PointerType*>(getType())) { */
/*         if (PT->isArrayPointer && valueName.compare("0") != 0) { */
/*             ret += "("; */
/*             for (unsigned i = 0; i < PT->levels; i++) { */
/*                 ret += "*"; */
/*             } */
/*             ret += valueName + ")"; */
/*         } else { */
/*             return ret + " " + valueName + ";"; */
/*         } */

/*         if (PT->isArrayPointer) { */
/*             ret = ret + PT->sizes; */
/*         } */


/*     } else { */
/*         ret += " " + valueName; */
/*     } */

/*     return ret + ";"; */
/* } */

void GlobalValue::accept(ExprVisitor& visitor) {
    visitor.visit(*this);
}

IfExpr::IfExpr(Expr* cmp, const std::string& trueBlock, const std::string& falseBlock)
    : cmp(cmp),
      trueBlock(trueBlock),
      falseBlock(falseBlock) {}

IfExpr::IfExpr(const std::string &trueBlock)
    : cmp(nullptr),
      trueBlock(trueBlock),
      falseBlock("") {}

void IfExpr::accept(ExprVisitor& visitor) {
    visitor.visit(*this);
}

SwitchExpr::SwitchExpr(Expr* cmp, const std::string &def, std::map<int, std::string> cases)
    : cmp(cmp),
      def(def),
      cases(cases) {}

void SwitchExpr::accept(ExprVisitor& visitor) {
    visitor.visit(*this);
}

AsmExpr::AsmExpr(const std::string& inst, const std::vector<std::pair<std::string, Expr*>>& output, const std::vector<std::pair<std::string, Expr*>>& input, const std::string& clobbers)
    : inst(inst),
      output(output),
      input(input),
      clobbers(clobbers) {}

void AsmExpr::addOutputExpr(Expr* expr, unsigned pos) {
    for (unsigned i = pos; i < output.size(); i++) {
        if (!output[i].second) {
            output[i].second = expr;
            break;
        }
    }
}

void AsmExpr::accept(ExprVisitor& visitor) {
    visitor.visit(*this);
}

CallExpr::CallExpr(Expr* funcValue, const std::string &funcName, std::vector<Expr*> params, std::unique_ptr<Type> type)
    : funcName(funcName),
      params(params),
      funcValue(funcValue) {
    setType(std::move(type));
}

/* std::string CallExpr::paramsToString() const { */
/*     std::string ret; */

/*     if (funcName.compare("va_start") == 0 || funcName.compare("va_end") == 0) { */
/*         ret += "(void*)("; */
/*     } */

/*     bool first = true; */
/*     for (auto param : params) { */
/*         if (first) { */
/*             ret += param->toString(); */
/*             if (funcName.compare("va_start") == 0 || funcName.compare("va_end") == 0) { */
/*                 ret += ")"; */
/*             } */
/*         } else { */
/*             ret += ", " + param->toString(); */
/*         } */

/*         first = false; */
/*     } */

/*     return ret; */
/* } */

void CallExpr::accept(ExprVisitor& visitor) {
    visitor.visit(*this);
}

PointerShift::PointerShift(std::unique_ptr<Type> ptrType, Expr* pointer, Expr* move)
    : ptrType(std::move(ptrType)),
      pointer(pointer),
      move(move) {
    if (auto PT = dynamic_cast<PointerType*>(this->ptrType.get())) {
        setType(PT->type->clone());
    }
}

void PointerShift::accept(ExprVisitor& visitor) {
    visitor.visit(*this);
}

GepExpr::GepExpr(std::vector<std::unique_ptr<Expr>>& indices) {
    for (auto& index : indices) {
        this->indices.push_back(std::move(index));
    }

    setType(this->indices[this->indices.size() - 1]->getType()->clone());
}

void GepExpr::accept(ExprVisitor& visitor) {
    visitor.visit(*this);
}

SelectExpr::SelectExpr(Expr* comp, Expr* l, Expr* r) :
    left(l),
    right(r),
    comp(comp) {
    setType(l->getType()->clone());
}

void SelectExpr::accept(ExprVisitor& visitor) {
    visitor.visit(*this);
}
