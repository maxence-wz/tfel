/*!
 * \file   MFrontPleiadesLawInterface.cxx
 * \brief
 * \author Helfer Thomas
 * \date   06 mai 2008
 */

#include<sstream>
#include<stdexcept>

#include"TFEL/System/System.hxx"

#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontHeader.hxx"
#include"MFront/MFrontPleiadesLawInterface-2.0.hxx"
#include"MFront/MFrontLawInterfaceProxy.hxx"

namespace mfront {

std::string MFrontPleiadesLawInterface::getName(void) {
  return "pleiades-2.0";
  // TODO: make a MFrontPleiadesLawInterface.cxx.in and use @PACKAGE_VERSION@
}

MFrontPleiadesLawInterface::MFrontPleiadesLawInterface() :
  verboseMode(false), debugMode(false), warningMode(false) {
}

void MFrontPleiadesLawInterface::reset(void) {
} // end of MFrontPleiadesLawInterface::reset(void)

void MFrontPleiadesLawInterface::setVerboseMode(void) {
  this->verboseMode = true;
}

void MFrontPleiadesLawInterface::setWarningMode(void) {
  this->warningMode = true;
}

void MFrontPleiadesLawInterface::setDebugMode(void) {
  this->debugMode = true;
}

std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
MFrontPleiadesLawInterface::treatKeyword(const std::string&,
                                         tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
                                         const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator) {
  using namespace std;
  return make_pair(false,current);
} // end of treatKeyword

MFrontPleiadesLawInterface::~MFrontPleiadesLawInterface() {
}

std::map<std::string,std::vector<std::string> >
MFrontPleiadesLawInterface::getGlobalDependencies(const std::string& lib,
                                                  const std::string& mat,
                                                  const std::string&) {
  using namespace std;
  map<string,vector<string> > libs;
  libs["libPleiades"+getMaterialLawLibraryNameBase(lib,mat)].push_back("-lm");
  return libs;
} // end of MFrontPleiadesLawInterface::getGeneratedSources

std::map<std::string,std::vector<std::string> >
MFrontPleiadesLawInterface::getGlobalIncludes(const std::string& lib,
                                              const std::string& mat,
                                              const std::string&) {
  using namespace std;
  map<string,vector<string> > incs;
  incs["libPleiades"+getMaterialLawLibraryNameBase(lib,mat)].push_back("`pleiades-config --includes`\n");
  return incs;
} // end of MFrontPleiadesLawInterface::getGeneratedSources

std::map<std::string,std::vector<std::string> >
MFrontPleiadesLawInterface::getGeneratedSources(const std::string& lib,
                                                const std::string& mat,
                                                const std::string& className) {
  using namespace std;
  map<string,vector<string> > src;
  string pleiadeslib = "libPleiades"+getMaterialLawLibraryNameBase(lib,mat);
  if(!mat.empty()){
    src[pleiadeslib].push_back(mat+"_"+className+"-pleiades.cpp");
  } else {
    src[pleiadeslib].push_back(className+"-pleiades.cpp");
  }
  return src;
} // end of MFrontPleiadesLawInterface::getGeneratedSources

std::vector<std::string>
MFrontPleiadesLawInterface::getGeneratedIncludes(const std::string&,
                                                 const std::string&,
                                                 const std::string&) {
  using namespace std;
  vector<string> incs;
  incs.push_back(this->headerFileName.substr(8));
  return incs;
} // end of MFrontPleiadesLawInterface::getGeneratedIncludes

std::map<std::string,std::vector<std::string> >
MFrontPleiadesLawInterface::getLibrariesDependencies(const std::string& lib,
                                                     const std::string& mat,
                                                     const std::string&) {
  using namespace std;
  map<string,vector<string> > libs;
  string pleiadeslib = "libPleiades"+getMaterialLawLibraryNameBase(lib,mat);
  libs[pleiadeslib].push_back("-lm");
  return libs;
} // end of MFrontPleiadesLawInterface::getLibrariesDependencies()

std::map<std::string,
std::pair<std::vector<std::string>,
std::vector<std::string> > >
MFrontPleiadesLawInterface::getSpecificTargets(const std::string&,
                                               const std::string&,
                                               const std::string&,
                                               const std::vector<std::string>&) {
  using namespace std;
  return map<string,pair<vector<string>,vector<string> > >();
} // end of MFrontPleiadesLawInterface::getSpecificTargets

void MFrontPleiadesLawInterface::writeOutputFiles(const std::string& file,
                                                  const std::string& ,
                                                  const std::string& material,
                                                  const std::string& className,
                                                  const std::string& author,
                                                  const std::string& date,
                                                  const std::string& description,
                                                  const std::string& includes,
                                                  const std::string& output,
                                                  const VarContainer& inputs,
                                                  const std::vector<std::string>& materialLaws,
                                                  const std::map<std::string,std::string>& glossaryNames,
                                                  const std::map<std::string,std::string>& entryNames,
                                                  const StaticVarContainer& staticVars,
                                                  const std::vector<std::string>& params,
                                                  const std::map<std::string,double>& paramValues,
                                                  const LawFunction& function,
                                                  const std::vector<VariableBoundsDescription>& bounds,
                                                  const std::vector<VariableBoundsDescription>& physicalBounds,
                                                  const bool,
                                                  const std::vector<std::string>&) {
  using namespace std;
  using namespace tfel::system;
  string dir;
  vector<string>::const_iterator p;
  systemCall::mkdir("include/Pleiades");
  systemCall::mkdir("include/Pleiades/Metier");
  systemCall::mkdir("include/Pleiades/Metier/MaterialProperty");
  string name;
  if(material.empty()){
    name = className;
  } else {
    name = material+"_"+className;
  }
  this->headerFileName  = "include/Pleiades/Metier/MaterialProperty/"+name;
  this->headerFileName += "-pleiades.hh";
  this->srcFileName  = "src/" + name;
  this->srcFileName += "-pleiades.cpp";
  this->headerFile.open(this->headerFileName.c_str());
  if(!this->headerFile){
    string msg("MFrontMaterialLawParser::writeOutputFiles : ");
    msg += "unable to open ";
    msg += this->headerFileName;
    msg += " for writing output file.";
    throw(runtime_error(msg));
  }
  this->headerFile.exceptions(ios::badbit|ios::failbit);
  this->srcFile.open(this->srcFileName.c_str());
  if(!this->srcFile){
    string msg("MFrontMaterialLawParser::writeOutputFiles : ");
    msg += "unable to open ";
    msg += this->srcFileName;
    msg += " for writing output file.";
    throw(runtime_error(msg));
  }
  this->srcFile.exceptions(ios::badbit|ios::failbit);
  this->writeHeaderFile(file,name,author,date,description,includes,inputs);
  this->writeSrcFile(file,name,author,date,output,
                     inputs,materialLaws,glossaryNames,
                     entryNames,staticVars,params,paramValues,
                     function,bounds,physicalBounds);
} // end of MFrontPleiadesLawInterface::writeOutputFiles

void MFrontPleiadesLawInterface::writeHeaderFile(const std::string&,
                                                 const std::string& name,
                                                 const std::string& author,
                                                 const std::string& date,
                                                 const std::string& description,
                                                 const std::string& includes,
                                                 const VarContainer& inputs) {
  using namespace std;
  VarContainer::const_iterator p;
  StaticVarContainer::const_iterator p2;
  vector<string>::const_iterator p3;
  vector<string>::const_iterator p3e;
  VarContainer::const_iterator p4;
  vector<string>::const_reverse_iterator p5;
  vector<string>::const_reverse_iterator p5e;
  map<string,double>::const_iterator p6;
  this->headerFile << "/*!" << endl;
  this->headerFile << "* \\file   " << this->headerFileName  << endl;
  this->headerFile << "* \\brief  " << "this file declares the "
          << name << " MaterialLaw.\n";
  this->headerFile << "*         File generated by ";
  this->headerFile << MFrontHeader::getVersionName() << " ";
  this->headerFile << "version " << MFrontHeader::getVersionNumber();
  this->headerFile << endl;
  if(!author.empty()){
    this->headerFile << "* \\author " << author << endl;
  }
  if(!date.empty()){
    this->headerFile << "* \\date   " << date       << endl;
  }
  if(!description.empty()){
    this->headerFile << description << endl;
  }
  this->headerFile << " */\n\n";

  this->headerFile << "#ifndef _PLEIADES_"
          << makeUpperCase(name)
          << "_HH\n";
  this->headerFile << "#define _PLEIADES_"
          << makeUpperCase(name)
          << "_HH\n\n";

//  writeExportDirectives(this->headerFile);

  this->headerFile << "#include<cmath>\n";
  if(!includes.empty()){
    this->headerFile << includes << endl;
  }
  this->headerFile << "#include \"Pleiades/Metier/MaterialProperty/MaterialPropertyBase.hh\"\n\n";
  this->headerFile << "namespace Pleiades\n{\n";
  //  this->headerFile << "\nstruct MFRONT_SHAREDOBJ " << name << endl;

  this->headerFile << "\nclass " << name << endl;
  this->headerFile << ": public MaterialPropertyBase\n";
  this->headerFile << "{\n\n";
  this->headerFile << "public:\n";
  this->headerFile << name << "();\n";
  this->headerFile << "void declare();\n ";
  this->headerFile << "void compute();\n ";
  this->headerFile << "double operator()(";
  if(!inputs.empty()){
    for(p4=inputs.begin();p4!=inputs.end();){
      this->headerFile << "const double& " << p4->name ;
      if((++p4)!=inputs.end()){
        this->headerFile << ",";
      }
    }
  } else {
    this->headerFile << "void";
  }
  this->headerFile << ") const;\n\n";
  // Disable copy constructor and assignement operator
  this->headerFile << "private:\n\n";
  this->headerFile << "//! Copy constructor (disabled)\n";
  this->headerFile << name << "(const "
          << name << "&);\n\n";
  this->headerFile << "//! Assignement operator (disabled)\n";
  this->headerFile << name << "&\n"
          << "operator=(const "
          << name << "&);\n\n";
  this->headerFile << "}; // end of class " << name << endl << endl;

  this->headerFile << "} // end of namespace Pleiades\n\n";
  this->headerFile << "#endif /* _PLEIADES_"
          << makeUpperCase(name)
          << "_HH */\n";
  this->headerFile.close();
} // end of MFrontPleiadesLawInterface::writeHeaderFile(void)

void
MFrontPleiadesLawInterface::writeSrcFile(const std::string& file,
                                         const std::string& name,
                                         const std::string& author,
                                         const std::string& date,
                                         const std::string& output,
                                         const VarContainer& inputs,
                                         const std::vector<std::string>& materialLaws,
                                         const std::map<std::string,std::string>& glossaryNames,
                                         const std::map<std::string,std::string>& entryNames,
                                         const StaticVarContainer& staticVars,
                                         const std::vector<std::string>& params,
                                         const std::map<std::string,double>& paramValues,
                                         const LawFunction& function,
                                         const std::vector<VariableBoundsDescription>& bounds,
                                         const std::vector<VariableBoundsDescription>& physicalBounds) {
  using namespace std;
  vector<string>::const_iterator p;
  vector<string>::const_iterator pe;
  StaticVarContainer::const_iterator p2;
  VarContainer::const_iterator p3;
  map<string,string>::const_iterator p4;
  vector<string>::const_reverse_iterator p5;
  vector<string>::const_reverse_iterator p5e;
  vector<VariableBoundsDescription>::const_iterator p6;
  map<string,double>::const_iterator p7;
  this->srcFile << "/*!" << endl;
  this->srcFile << "* \\file   " << this->srcFileName  << endl;
  this->srcFile << "* \\brief  " << "this file implements the "
          << name << " MaterialLaw.\n";
  this->srcFile << "*         File generated by ";
  this->srcFile << MFrontHeader::getVersionName() << " ";
  this->srcFile << "version " << MFrontHeader::getVersionNumber();
  this->srcFile << endl;
  if(!author.empty()){
    this->srcFile << "* \\author " << author << endl;
  }
  if(!date.empty()){
    this->srcFile << "* \\date   " << date       << endl;
  }
  this->srcFile << " */\n\n";
  this->srcFile << "#include<string>\n";
  this->srcFile << "#include<cmath>\n\n";
  this->srcFile << "#include\"Pleiades/Examplars/ClassProxy.hh\"\n";
  this->srcFile << "#include\"Pleiades/Exceptions/pexceptions.hh\"\n";
  this->srcFile << "#include\"Pleiades/Metier/Glossaire/pglossaire.hh\"\n";
  this->srcFile << "#include\"Pleiades/Metier/MaterialProperty/" << name << "-pleiades.hh\"\n";
  this->srcFile << "namespace Pleiades {\n\n";
  this->srcFile << name << "::" << name <<"(){}\n\n";

  // declare
  this->srcFile << "void ";
  this->srcFile << name
                  << "::declare() {\n";
    this->srcFile << "using namespace Pleiades::PGlossaire;\n";

    string oname;
    if((p4=glossaryNames.find(output))!=glossaryNames.end()){
      oname = "GlossaireField::"+p4->second;
    } else if ((p4=entryNames.find(output))!=entryNames.end()){
      oname = "\"" + p4->second + "\"";
    } else {
      this->srcFile << "AFF(\"WARNING !!! Glossary name not defined for output field : " << output << " in " << name << "\");" << endl;
      oname = "\""+output+"\"";
    }
    this->srcFile << "declareField<double>(\"OutputField\", OUT);\n";
    this->srcFile << "setFieldName(\"OutputField\", " << oname << ");\n";

    string iname;
  for(p3=inputs.begin();p3!=inputs.end();++p3){
    if((p4=glossaryNames.find(p3->name))!=glossaryNames.end()){
      iname = "GlossaireField::"+p4->second;
    } else if((p4=entryNames.find(p3->name))!=entryNames.end()){
      iname = "\""+p4->second+"\"";
    } else {
      iname = "\""+p3->name+"\"";
    }

    this->srcFile << "declareField<double>(\"" << p3->name << "\", " << "IN" << ");\n";
    this->srcFile << "setFieldName(\"" << p3->name << "\", " << iname << ");\n";
  }

  this->srcFile << "}\n\n";

  // Compute
  this->srcFile << "void ";
  this->srcFile << name << "::compute(void) {\n";
  this->srcFile << "  apply(*this, getOutputField<double>(\"OutputField\")";
  for(p3=inputs.begin();p3!=inputs.end();){
    this->srcFile << ", getField<double>(\"" << p3->name << "\")";
    ++p3;
  }
  this->srcFile << ");\n";
  this->srcFile << "} // end of " << name << "::compute\n\n";
  // Law
  this->srcFile << "double ";
  this->srcFile << name << "::operator()(";
  if(!inputs.empty()){
    for(p3=inputs.begin();p3!=inputs.end();){
      this->srcFile << "const double& " << p3->name;
      if((++p3)!=inputs.end()){
        this->srcFile << ",";
      }
    }
  } else {
    this->srcFile << "void";
  }
  this->srcFile << ") const {\n";
  this->srcFile << "using namespace std;" << endl;
  this->srcFile << "using namespace Pleiades::PGlossaire;\n";

  // material laws
  writeMaterialLaws("MFrontPleiadesLawInterface::writeSrcFile",
                    this->srcFile,materialLaws);
  // static variables
  writeStaticVariables("MFrontPleiadesLawInterface::writeSrcFile",
                       srcFile,staticVars,file);
  if(!params.empty()){
    for(p=params.begin();p!=params.end();++p){
      p7 = paramValues.find(*p);
      if(p7==paramValues.end()){
        string msg("MFrontPleiadesLawInterface::writeCSrcFile : ");
        msg += "internal error (can't find value of parameter " + *p + ")";
        throw(runtime_error(msg));
      }
      this->srcFile << "static const double " << *p
              << " = " << p7->second << ";\n";
    }
  }
  this->srcFile << "double " << output << ";\n";
  if((!physicalBounds.empty())||
          (!bounds.empty())){
    this->srcFile << "#ifndef NO_PLEIADES_BOUNDS_CHECK\n";
  }
  if(!physicalBounds.empty()){
    this->srcFile << "// treating physical bounds\n";
    for(p6=physicalBounds.begin();
            p6!=physicalBounds.end();++p6){
      string fname;
      if((p4=glossaryNames.find(p6->varName))!=glossaryNames.end()){
        fname = "GlossaireField::"+p4->second;
      } else if((p4=entryNames.find(p6->varName))!=entryNames.end()){
        fname = "\""+p4->second+"\"";
      } else {
        fname = "\""+p6->varName+"\"";
      }
      if(p6->boundsType==VariableBoundsDescription::Lower){
        this->srcFile << "if(" << p6->varName<< " < "<< p6->lowerBound << "){\n";
        this->srcFile << "string msg (\"" << name << "::compute : \");\n"
                << "msg += " << fname << ";\n"
                << "msg += \" is below its physical lower bound.\";\n";
        this->srcFile << "PLEIADES_THROW(msg);\n";
        this->srcFile << "}\n";
      } else if(p6->boundsType==VariableBoundsDescription::Upper){
        this->srcFile << "if(" << p6->varName<< " > "<< p6->upperBound << "){\n";
        this->srcFile << "string msg (\"" << name << "::compute : \");\n"
                << "msg += " << fname << ";\n"
                << "msg += \" is over its physical upper bound.\";\n";
        this->srcFile << "PLEIADES_THROW(msg);\n";
        this->srcFile << "}\n";
      } else {
        this->srcFile << "if((" << p6->varName<< " < "<< p6->lowerBound << ")||"
                << "(" << p6->varName<< " > "<< p6->upperBound << ")){\n";
        this->srcFile << "if(" << p6->varName<< " < " << p6->lowerBound << "){\n";
        this->srcFile << "string msg (\"" << name << "::compute : \");\n"
                << "msg += " << fname << ";\n"
                << "msg += \" is below its physical lower bound.\";\n";
        this->srcFile << "PLEIADES_THROW(msg);\n";
        this->srcFile << "} else {\n";
        this->srcFile << "string msg (\"" << name << "::compute : \");\n"
                << "msg += " << fname << ";\n"
                << "msg += \" is over its physical upper bound.\";\n";
        this->srcFile << "PLEIADES_THROW(msg);\n";
        this->srcFile << "}\n";
        this->srcFile << "}\n";
      }
    }
  }
  if(!bounds.empty()){
    this->srcFile << "// treating standard bounds\n";
    for(p6=bounds.begin();
            p6!=bounds.end();++p6){
      string fname;
      if((p4=glossaryNames.find(p6->varName))!=glossaryNames.end()){
        fname = "GlossaireField::"+p4->second;
      } else if((p4=entryNames.find(p6->varName))!=entryNames.end()){
        fname = "\""+p4->second+"\"";
      } else {
        fname = "\""+p6->varName+"\"";
      }
      if(p6->boundsType==VariableBoundsDescription::Lower){
        this->srcFile << "if(" << p6->varName<< " < "<< p6->lowerBound << "){\n";
        this->srcFile << "string msg(\"" << name << "::compute : value of\");\n";
        this->srcFile << "msg += " << fname << ";\n";
        this->srcFile << "msg += \" is below its lower bound\";\n";
        this->srcFile << "treatOutOfBounds(msg);\n";
        this->srcFile << "}\n";
      } else if(p6->boundsType==VariableBoundsDescription::Upper){
        this->srcFile << "if(" << p6->varName<< " > "<< p6->upperBound << "){\n";
        this->srcFile << "string msg(\"" << name << "::compute : value of\");\n";
        this->srcFile << "msg += " << fname << ";\n";
        this->srcFile << "msg += \" is over its upper bound\";\n";
        this->srcFile << "treatOutOfBounds(msg);\n";
        this->srcFile << "}\n";
      } else {
        this->srcFile << "if((" << p6->varName<< " < "<< p6->lowerBound << ")||"
                << "(" << p6->varName<< " > "<< p6->upperBound << ")){\n";
        this->srcFile << "string msg(\"" << name << "::compute : value of\");\n";
        this->srcFile << "msg += " << fname << ";\n";
        this->srcFile << "msg += \" is out of bounds\";\n";
        this->srcFile << "treatOutOfBounds(msg);\n";
        this->srcFile << "}\n";
      }
    }
  }
  if((!physicalBounds.empty())||
          (!bounds.empty())){
    this->srcFile << "#endif /* NO_PLEIADES_BOUNDS_CHECK */\n";
  }
  this->srcFile << function.body;
  this->srcFile << "return " << output << ";\n";
  this->srcFile << "} // end of " << name << "::law\n\n";

  this->srcFile << "GENERATE_PROXY(IMaterialProperty,";
  this->srcFile << name << ");\n";

//  this->srcFile << "GENERATE_PROXY2(IFunction,";
//  for(p  = names.begin();
//          p != names.end();){
//    this->srcFile << *p;
//    if((++p)!=names.end()){
//      this->srcFile << "::";
//    }
//  }
//  this->srcFile << ",";
//  for(p  = names.begin();
//          p != names.end();){
//    this->srcFile << *p;
//    if((++p)!=names.end()){
//      this->srcFile << "_";
//    }
//  }
//  this->srcFile << ");\n\n";
  this->srcFile << "} // end of namespace Pleiades\n";
  this->srcFile.close();
} // end of MFrontPleiadesLawInterface::writeSrcFile(void)

MFrontLawInterfaceProxy<MFrontPleiadesLawInterface>         pleiadesLawProxy;

} // end of namespace mfront
