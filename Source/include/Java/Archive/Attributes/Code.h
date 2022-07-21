#ifndef SUPERJET_CODE_H
#define SUPERJET_CODE_H

#include "Java/Types.h"
#include "Java/Archive/AttributeInfo.h"
#include "Java/Archive/ConstantPool.h"
#include "Java/JVM/Runtime/Operations.h"
#include <string>
#include <vector>

namespace SuperJet::Java::Archive::Attributes
{
    class Code
    {
        static constexpr auto CODE_ATTRIBUTE_NAME = "Code";

        class ExceptionTableEntry
        {
        public:
            ExceptionTableEntry(const JVM::u2 inStartPc, JVM::u2 inEndPc, const JVM::u2 inHandlePc, const JVM::u2 inCatchType) : startPc(inStartPc), endPc(inEndPc), handlerPc(inHandlePc), catchType(inCatchType)
            {
            }

            JVM::u2 getStartPc() const
            {
                return startPc;
            }

            JVM::u2 getEndPc() const
            {
                return endPc;
            }

            JVM::u2 getHandlerPc() const
            {
                return handlerPc;
            }

            JVM::u2 getCatchType() const
            {
                return catchType;
            }

        protected:
            JVM::u2 startPc;
            JVM::u2 endPc;
            JVM::u2 handlerPc;
            JVM::u2 catchType;
        };

    public:
        Code(const ConstantPool& constantPool, const AttributeInfo& attributeInfo);

    protected:
        JVM::u2 attributeNameIndex;
        JVM::u4 attributeLength;
        JVM::u2 maxStack;
        JVM::u2 maxLocals;
        std::vector<std::shared_ptr<JVM::Runtime::Operation>> code;
        std::vector<ExceptionTableEntry> exceptionTable;
        std::vector<Archive::AttributeInfo> attributes;
    };
}

#endif //SUPERJET_CODE_H