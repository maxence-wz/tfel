/*!
 * \file   mfront/src/BehaviourCodeGeneratorBase-SrcFile.cxx
 * \brief  This file implements the methods of the BehaviourCodeGeneratorBase
 * associated with the generation of the source file
 * \date   22/02/2024
 */

#include <ostream>
#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "MFront/MFrontHeader.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/PedanticMode.hxx"
#include "MFront/SupportedTypes.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/BehaviourCodeGeneratorBase.hxx"

namespace mfront {

  void BehaviourCodeGeneratorBase::writeSrcFile(std::ostream& os) const {
    this->writeSrcFileHeader(os);
    this->writeSrcFileUserDefinedCode(os);
    this->writeNamespaceBegin(os);
    this->writeSrcFileBehaviourProfiler(os);
    this->writeSrcFileParametersInitializers(os);
    this->writeNamespaceEnd(os);
  }  // end of writeSrcFile

  void BehaviourCodeGeneratorBase::checkSrcFile(std::ostream& os) const {
    if ((!os) || (!os.good())) {
      this->throwRuntimeError("BehaviourCodeGeneratorBase::checkSrcFile",
                              "output file is not valid");
    }
  }

  void BehaviourCodeGeneratorBase::writeSrcFileHeader(std::ostream& os) const {
    this->checkSrcFile(os);
    os << "/*!\n"
       << "* \\file   " << this->getSrcFileName() << '\n'
       << "* \\brief  "
       << "this file implements the " << this->bd.getClassName()
       << " Behaviour.\n"
       << "*         File generated by " << MFrontHeader::getVersionName()
       << " "
       << "version " << MFrontHeader::getVersionNumber() << '\n';
    if (!this->fd.authorName.empty()) {
      os << "* \\author " << this->fd.authorName << '\n';
    }
    if (!this->fd.date.empty()) {
      os << "* \\date   " << this->fd.date << '\n';
    }
    os << " */\n\n";
    if (this->bd.hasParameters()) {
      os << "#include<string>\n"
         << "#include<cstring>\n"
         << "#include<sstream>\n"
         << "#include<fstream>\n"
         << "#include<stdexcept>\n\n";
    }
    os << "#include\"TFEL/Raise.hxx\"\n"
       << "#include\"" << this->getBehaviourDataFileName() << "\"\n"
       << "#include\"" << this->getIntegrationDataFileName() << "\"\n"
       << "#include\"" << this->getBehaviourFileName() << "\"\n\n";
  }  // end of writeSrcFileHeader()

  void BehaviourCodeGeneratorBase::writeSrcFileUserDefinedCode(
      std::ostream& os) const {
    this->checkSrcFile(os);
    const auto& s = this->bd.getSources();
    if (!s.empty()) {
      os << s << "\n\n";
    }
  }  // end of writeSrcFileUserDefinedCode

  void BehaviourCodeGeneratorBase::writeSrcFileParametersInitializers(
      std::ostream& os) const {
    // useless and paranoid test
    if ((areParametersTreatedAsStaticVariables(this->bd)) ||
        (!this->bd.hasParameters())) {
      return;
    }
    auto hs = this->bd.getDistinctModellingHypotheses();
    hs.insert(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    for (const auto& h : hs) {
      if (this->bd.hasParameters(h)) {
        this->writeSrcFileParametersInitializer(os, h);
      }
    }
  }  // end of writeSrcFileParametersInitializer

  static void BehaviourCodeGeneratorBase_writeConverter(
      std::ostream& f,
      const std::string& cname,
      const std::string& type,
      const std::string& type2) {
    f << type << '\n'
      << cname << "::get" << type2 << "(const std::string& n,\n"
      << "const std::string& v)\n"
      << "{\n"
      << type << " value;\n"
      << "std::istringstream converter(v);\n"
      << "converter >> value;\n"
      << "tfel::raise_if(!converter||(!converter.eof()),\n"
      << "\"" << cname << "::get" << type2 << ": \"\n"
      << "\"can't convert '\"+v+\"' to " << type << " "
      << "for parameter '\"+ n+\"'\");\n"
      << "return value;\n"
      << "}\n\n";
  }

  void BehaviourCodeGeneratorBase::writeSrcFileParametersInitializer(
      std::ostream& os, const Hypothesis h) const {
    // useless and paranoid test
    if ((areParametersTreatedAsStaticVariables(this->bd)) ||
        (!this->bd.hasParameters())) {
      return;
    }
    this->checkBehaviourFile(os);
    // treating the default case
    bool rp = false;   // real    parameter found
    bool ip = false;   // integer parameter found
    bool up = false;   // unsigned short parameter found
    bool rp2 = false;  // real    parameter found
    bool ip2 = false;  // integer parameter found
    bool up2 = false;  // unsigned short parameter found
    std::string cname(this->bd.getClassName());
    if (h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      cname += ModellingHypothesis::toString(h);
    }
    cname += "ParametersInitializer";
    std::string dcname(this->bd.getClassName() + "ParametersInitializer");
    os << cname << "&\n"
       << cname << "::get()\n"
       << "{\n"
       << "static " << cname << " i;\n"
       << "return i;\n"
       << "}\n\n";
    os << cname << "::" << cname << "()\n"
       << "{\n";
    for (const auto& p : this->bd.getBehaviourData(h).getParameters()) {
      if (p.type == "int") {
        ip = true;
        if ((h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) ||
            ((h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) &&
             (!this->bd.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                                     p.name)))) {
          ip2 = true;
          os << "this->" << p.name << " = "
             << this->bd.getIntegerParameterDefaultValue(h, p.name) << ";\n";
        }
      } else if (p.type == "ushort") {
        up = true;
        if ((h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) ||
            ((h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) &&
             (!this->bd.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                                     p.name)))) {
          up2 = true;
          os << "this->" << p.name << " = "
             << this->bd.getUnsignedShortParameterDefaultValue(h, p.name)
             << ";\n";
        }
      } else {
        const auto f = SupportedTypes::getTypeFlag(p.type);
        if (f != SupportedTypes::SCALAR) {
          this->throwRuntimeError(
              "BehaviourCodeGeneratorBase::writeSrcFileParametersInitializer",
              "unsupported parameter type '" + p.type +
                  "' "
                  "for parameter '" +
                  p.name + "'");
        }
        rp = true;
        if ((h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) ||
            ((h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) &&
             (!this->bd.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                                     p.name)))) {
          rp2 = true;
          if (p.arraySize == 1u) {
            os << "this->" << p.name << " = "
               << this->bd.getFloattingPointParameterDefaultValue(h, p.name)
               << ";\n";
          } else {
            for (unsigned short i = 0; i != p.arraySize; ++i) {
              os << "this->" << p.name << "[" << i << "] = "
                 << this->bd.getFloattingPointParameterDefaultValue(h, p.name,
                                                                    i)
                 << ";\n";
            }
          }
        }
      }
    }
    if (allowsParametersInitializationFromFile(this->bd)) {
      os << "// Reading parameters from a file\n";
      os << cname << "::readParameters(*this,\""
         << getParametersFileName(this->bd) << "\");\n";
      if (h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
        os << cname << "::readParameters(*this,\""
           << getParametersFileName(this->bd, h) << "\");\n";
      }
    }
    os << "}\n\n";
    auto write_if = [&os](bool& b) {
      if (b) {
        os << "if(";
        b = false;
      } else {
        os << "} else if(";
      }
    };
    if (rp) {
      os << "void\n"
         << cname << "::set(const char* const key,\nconst double v)"
         << "{\n"
         << "using namespace std;\n";
      bool first = true;
      for (const auto& p : this->bd.getBehaviourData(h).getParameters()) {
        if ((p.type == "int") || (p.type == "ushort")) {
          continue;
        }
        const auto f = SupportedTypes::getTypeFlag(p.type);
        if (f != SupportedTypes::SCALAR) {
          this->throwRuntimeError(
              "BehaviourCodeGeneratorBase::writeSrcFileParametersInitializer",
              "unsupported parameter type '" + p.type +
                  "' "
                  "for parameter '" +
                  p.name + "'");
        }
        if (p.arraySize == 1u) {
          write_if(first);
          os << "::strcmp(\"" + this->bd.getExternalName(h, p.name) +
                    "\",key)==0){\n";
          if ((h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) ||
              ((h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) &&
               (!this->bd.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                                       p.name)))) {
            os << "this->" << p.name << " = v;\n";
          } else {
            os << dcname << "::get().set(\""
               << this->bd.getExternalName(h, p.name) << "\",v);\n";
          }
        } else {
          for (unsigned short i = 0; i != p.arraySize; ++i) {
            write_if(first);
            const auto vn = p.name + '[' + std::to_string(i) + ']';
            const auto en = this->bd.getExternalName(h, p.name) + '[' +
                            std::to_string(i) + ']';
            os << "::strcmp(\"" + en + "\",key)==0){\n";
            if ((h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) ||
                ((h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) &&
                 (!this->bd.hasParameter(
                     ModellingHypothesis::UNDEFINEDHYPOTHESIS, p.name)))) {
              os << "this->" << vn << " = v;\n";
            } else {
              os << dcname << "::get().set(\"" << en << "\",v);\n";
            }
          }
        }
      }
      os << "} else {\n";
      os << "tfel::raise(\"" << cname << "::set: \"\n"
         << "\" no parameter named "
         << "'\"+std::string(key)+\"'\");\n"
         << "}\n"
         << "}\n\n";
    }
    if (ip) {
      os << "void\n"
         << cname << "::set(const char* const key,\nconst int v)"
         << "{\n"
         << "using namespace std;\n";
      bool first = true;
      for (const auto& p : this->bd.getBehaviourData(h).getParameters()) {
        if (p.type == "int") {
          write_if(first);
          os << "::strcmp(\"" + this->bd.getExternalName(h, p.name) +
                    "\",key)==0){\n";
          if ((h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) ||
              ((h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) &&
               (!this->bd.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                                       p.name)))) {
            os << "this->" << p.name << " = v;\n";
          } else {
            os << dcname << "::get().set(\""
               << this->bd.getExternalName(h, p.name) << "\",v);\n";
          }
        }
      }
      os << "} else {\n";
      os << "tfel::raise(\"" << cname << "::set: \"\n"
         << "\"no parameter named "
         << "'\"+std::string(key)+\"'\");\n"
         << "}\n"
         << "}\n\n";
    }
    if (up) {
      os << "void\n"
         << cname << "::set(const char* const key,\nconst unsigned short v)"
         << "{\n"
         << "using namespace std;\n";
      bool first = true;
      for (const auto& p : this->bd.getBehaviourData(h).getParameters()) {
        if (p.type == "ushort") {
          write_if(first);
          os << "::strcmp(\"" + this->bd.getExternalName(h, p.name) +
                    "\",key)==0){\n";
          if ((h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) ||
              ((h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) &&
               (!this->bd.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                                       p.name)))) {
            os << "this->" << p.name << " = v;\n";
          } else {
            os << dcname << "::get().set(\""
               << this->bd.getExternalName(h, p.name) << "\",v);\n";
          }
        }
      }
      os << "} else {\n";
      os << "tfel::raise(\"" << cname << "::set: \"\n"
         << "\"no parameter named '\"+std::string(key)+\"'\");\n"
         << "}\n"
         << "}\n\n";
    }
    if (allowsParametersInitializationFromFile(this->bd)) {
      if (rp2) {
        BehaviourCodeGeneratorBase_writeConverter(os, cname, "double",
                                                  "Double");
      }
      if (ip2) {
        BehaviourCodeGeneratorBase_writeConverter(os, cname, "int", "Int");
      }
      if (up2) {
        BehaviourCodeGeneratorBase_writeConverter(os, cname, "unsigned short",
                                                  "UnsignedShort");
      }
      os << "void\n" << cname << "::readParameters(" << cname << "&";
      if (rp2 || ip2 || up2) {
        os << " pi";
      }
      os << ",const char* const fn)"
         << "{\n"
         << "auto tokenize = [](const std::string& line){\n"
         << "std::istringstream tokenizer(line);\n"
         << "std::vector<std::string> tokens;\n"
         << "std::copy(std::istream_iterator<std::string>(tokenizer),\n"
         << "std::istream_iterator<std::string>(),\n"
         << "std::back_inserter(tokens));\n"
         << "return tokens;\n"
         << "};\n"
         << "std::ifstream f(fn);\n"
         << "if(!f){\n"
         << "return;\n"
         << "}\n"
         << "size_t ln = 1u;\n"
         << "auto line = std::string{};\n"
         << "while(std::getline(f,line)){\n"
         << "auto tokens = tokenize(line);\n"
         << "auto throw_if = [ln = ln, line, fn](const bool mfront_c,\n"
         << "const std::string& mfront_error_message){\n"
         << "tfel::raise_if(mfront_c,\"" << cname << "::readParameters: \"\n"
         << "\"error at line '\"+std::to_string(ln)+\"' \"\n"
         << "\"while reading parameter file '\"+std::string(fn)+\"'\"\n"
         << "\"(\"+mfront_error_message+\")\");\n"
         << "};\n"
         << "if(tokens.empty()){\n"
         << "continue;\n"
         << "}\n"
         << "if(tokens[0][0]=='#'){\n"
         << "continue;\n"
         << "}\n"
         << "throw_if(tokens.size()!=2u,\"invalid number of tokens\");\n";
      bool first = true;
      for (const auto& p : this->bd.getBehaviourData(h).getParameters()) {
        const auto b =
            ((h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) ||
             ((h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) &&
              (!this->bd.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                                      p.name))));
        auto write = [this, &os, &p, &b, &dcname, &cname](
                         const std::string& vn, const std::string& en) {
          os << "\"" << en << "\"==tokens[0]){\n";
          if (b) {
            os << "pi." << vn << " = ";
            if (p.type == "int") {
              os << cname << "::getInt(tokens[0],tokens[1]);\n";
            } else if (p.type == "ushort") {
              os << cname << "::getUnsignedShort(tokens[0],tokens[1]);\n";
            } else {
              const auto f = SupportedTypes::getTypeFlag(p.type);
              if (f != SupportedTypes::SCALAR) {
                this->throwRuntimeError(
                    "BehaviourCodeGeneratorBase::"
                    "writeSrcFileParametersInitializer",
                    "invalid parameter type '" + p.type + "'");
              }
              os << cname << "::getDouble(tokens[0],tokens[1]);\n";
            }
          } else {
            os << dcname << "::get().set(\"" << en << "\",\n";
            if (p.type == "int") {
              os << dcname << "::getInt(tokens[0],tokens[1])";
            } else if (p.type == "ushort") {
              os << dcname << "::getUnsignedShort(tokens[0],tokens[1])";
            } else {
              const auto f = SupportedTypes::getTypeFlag(p.type);
              if (f != SupportedTypes::SCALAR) {
                this->throwRuntimeError(
                    "BehaviourCodeGeneratorBase::"
                    "writeSrcFileParametersInitializer",
                    "invalid parameter type '" + p.type + "'");
              }
              os << dcname << "::getDouble(tokens[0],tokens[1])";
            }
            os << ");\n";
          }
        };
        if (p.arraySize == 1u) {
          write_if(first);
          write(p.name, this->bd.getExternalName(h, p.name));
        } else {
          for (unsigned short i = 0; i != p.arraySize; ++i) {
            const auto vn = p.name + '[' + std::to_string(i) + ']';
            const auto en = this->bd.getExternalName(h, p.name) + '[' +
                            std::to_string(i) + ']';
            write_if(first);
            write(vn, en);
          }
        }
      }
      os << "} else {\n"
         << "throw_if(true,\"invalid parameter '\"+tokens[0]+\"'\");\n"
         << "}\n"
         << "}\n"
         << "}\n\n";
    }
  }  // end of writeSrcFileParametersInitializer

  void BehaviourCodeGeneratorBase::writeSrcFileBehaviourProfiler(
      std::ostream& os) const {
    if (this->bd.getAttribute(BehaviourData::profiling, false)) {
      this->checkSrcFile(os);
      os << "mfront::BehaviourProfiler&\n"
         << this->bd.getClassName() << "Profiler::getProfiler()\n"
         << "{\n"
         << "static mfront::BehaviourProfiler profiler(\""
         << this->bd.getClassName() << "\");\n"
         << "return profiler;\n"
         << "}\n\n";
    }
  }  // end of writeSrcFileBehaviourProfiler

}  // end of namespace mfront
