#include "Compiler/Compiler.h"
#include "Compiler/ClassInfoRerenceCollector.h"
#include "Compiler/Exceptions/RuntimeException.h"
#include "spdlog/spdlog.h"
#include "fmt/format.h"
#include "Java/Archive/Archive.h"

namespace SuperJet::Compiler
{
    Context::Context(const Environment& env) : environment(env)
    {
        for(const std::string& library : environment.libraries)
        {
            std::shared_ptr<SuperJet::Java::Archive::Jar> jar = std::make_shared<SuperJet::Java::Archive::Jar>(Java::Archive::Jar(library));

            for (ssize_t entryIndex = 0; entryIndex < jar->count(); entryIndex++)
            {
                const SuperJet::Java::Archive::Jar::Entry& entry = jar->open(entryIndex);
                if (!entry.isDirectory())
                {
                    if (std::filesystem::path(entry.getName()).extension() == Java::Archive::CLASS_FILE_EXTENSION)
                    {
                        const size_t extensionPosition = entry.getName().find(Java::Archive::CLASS_FILE_EXTENSION);
                        std::string entryNameWithoutExtension = entry.getName().substr(0, extensionPosition);

                        spdlog::debug(fmt::format("Found class \'{}\' in archive \'{}\'", entryNameWithoutExtension, jar->getLocation().string()));
                        classMap.emplace(entryNameWithoutExtension, jar);
                    }
                }
            }
        }
    }

    const std::filesystem::path& Context::getMainClass() const
    {
        return environment.mainClass;
    }

    std::shared_ptr<Java::Archive::Jar> Context::jarForClass(const std::filesystem::path& javaClassName)
    {
        auto find = classMap.find(javaClassName);
        if (find != classMap.end())
        {
            return classMap.at(javaClassName);
        }

        throw SuperJet::Compiler::RuntimeException(fmt::format("Can't find class \'{}\' in any of jar archives!", javaClassName.string()));
    }

    void ByteCodeCompiler::run()
    {
    }

    ByteCodeCompiler::ByteCodeCompiler(const Environment& env) : context(env)
    {
        ClassInfoReferenceCollector collector = ClassInfoReferenceCollector(context);
        dependencyGraph = collector.collect(context.getMainClass());
        return;
    }
}