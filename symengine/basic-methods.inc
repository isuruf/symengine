class StrPrinterFinal;
class EvalRealDoubleVisitor;

#define SYMENGINE_INCLUDE_METHODS(SUFFIX) \
virtual void accept(Visitor &v) const SUFFIX; \
virtual void accept(StrPrinterFinal &v) const SUFFIX; \
virtual void accept(EvalRealDoubleVisitor &v) const SUFFIX; \
virtual RCP<const Basic> diff(const RCP<const Symbol> &x) const SUFFIX;
