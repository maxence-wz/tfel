/*! 
 * \file  bindings/python/mfront/MTest.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 20 sept. 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<boost/python.hpp>

#include"MTest/MTest.hxx"
#include"MTest/Evolution.hxx"
#include"MTest/FunctionEvolution.hxx"
#include"MTest/CastemEvolution.hxx"
#include"MTest/CurrentState.hxx"
#include"MTest/StructureCurrentState.hxx"
#include"MTest/Constraint.hxx"
#include"MTest/ImposedThermodynamicForce.hxx"
#include"MTest/ImposedDrivingVariable.hxx"

static void
MTest_addEvolution(mtest::MTest& t,
		   const std::string&  n,
		   const mtest::real& v,
		   const bool b1,
		   const bool b2)
{
  using namespace mtest;
  std::shared_ptr<Evolution> pev(new ConstantEvolution(v));
  t.addEvolution(n,pev,b1,b2);
}

static void
MTest_addEvolution2(mtest::MTest& t,
		    const std::string&  n,
		    const std::map<mtest::real,
				   mtest::real>& v,
		    const bool b1,
		    const bool b2)
{
  using namespace std;
  using namespace mtest;
  using mtest::real;
  vector<real> tv(v.size());
  vector<real> ev(v.size());
  vector<real>::size_type i;
  map<real,real>::const_iterator pv;
  for(pv=v.begin(),i=0;pv!=v.end();++pv,++i){
    tv[i] = pv->first;
    ev[i] = pv->second;
  }
  shared_ptr<Evolution> pev(new LPIEvolution(tv,ev));
  t.addEvolution(n,pev,b1,b2);
}

static void
MTest_addEvolution3(mtest::MTest& t,
		    const std::string&  n,
		    const std::string& f,
		    const bool b1,
		    const bool b2)
{
  using namespace std;
  using namespace mtest;
  shared_ptr<Evolution> pev(new FunctionEvolution(f,t.getEvolutions()));
  t.addEvolution(n,pev,b1,b2);
}

static void
MTest_setMaterialProperty(mtest::MTest& t,
			  const std::string&  n,
			  const mtest::real& v,
			  const bool b)
{
  using namespace mtest;
  std::shared_ptr<Evolution> pev(new ConstantEvolution(v));
  t.setMaterialProperty(n,pev,b);
}

static void
MTest_setMaterialProperty2(mtest::MTest& t,
			   const std::string& n,
			   const std::string& f,
			   const bool b)
{
  using namespace std;
  using namespace mtest;
  using mtest::real;
  shared_ptr<Evolution> mpev;
  mpev = shared_ptr<Evolution>(new FunctionEvolution(f,t.getEvolutions()));
  t.setMaterialProperty(n,mpev,b);
}

static void
MTest_setMaterialProperty3(mtest::MTest& t,
		    const std::string&  n,
		    const std::string& f,
		    const bool b)
{
  using namespace std;
  using namespace mtest;
  shared_ptr<Evolution> pev(new FunctionEvolution(f,t.getEvolutions()));
  t.setMaterialProperty(n,pev,b);
}

static void
MTest_setCastemMaterialProperty(mtest::MTest& t,
				const std::string& n,
				const std::string& l,
				const std::string& f,
				const bool b)
{
  using namespace std;
  using namespace mtest;
  using mtest::real;
  shared_ptr<Evolution> mpev;
  mpev = shared_ptr<Evolution>(new CastemEvolution(l,f,t.getEvolutions()));
  t.setMaterialProperty(n,mpev,b);
}

static void
MTest_setExternalStateVariable(mtest::MTest& t,
			       const std::string&  n,
			       const mtest::real& v,
			       const bool b)
{
  using namespace mtest;
  std::shared_ptr<Evolution> pev(new ConstantEvolution(v));
  t.setExternalStateVariable(n,pev,b);
}

static void
MTest_setExternalStateVariable2(mtest::MTest& t,
				const std::string&  n,
				const std::map<mtest::real,
				mtest::real>& v,
				const bool b)
{
  using namespace std;
  using namespace mtest;
  using mtest::real;
  vector<real> tv(v.size());
  vector<real> ev(v.size());
  vector<real>::size_type i;
  map<real,real>::const_iterator pv;
  for(pv=v.begin(),i=0;pv!=v.end();++pv,++i){
    tv[i] = pv->first;
    ev[i] = pv->second;
  }
  shared_ptr<Evolution> pev(new LPIEvolution(tv,ev));
  t.setExternalStateVariable(n,pev,b);
}

static void
MTest_setExternalStateVariable3(mtest::MTest& t,
				const std::string&  n,
				const std::string& f,
				const bool b)
{
  using namespace std;
  using namespace mtest;
  shared_ptr<Evolution> pev(new FunctionEvolution(f,t.getEvolutions()));
  t.setExternalStateVariable(n,pev,b);
}

static void
MTest_setImposedThermodynamicForce(mtest::MTest& t,
				   const std::string&  n,
				   const mtest::real& v)
{
  using namespace std;
  using namespace mtest;
  using std::shared_ptr; 
  shared_ptr<Constraint> sc;
  shared_ptr<Evolution> sev(new ConstantEvolution(v));
  sc = shared_ptr<Constraint>(new ImposedThermodynamicForce(*(t.getBehaviour()),
								      t.getModellingHypothesis(),
								      n,sev));
  t.addEvolution(n,sev,false,true);
  t.addConstraint(sc);
} // end of MTest_setImposedThermodynamicForce

static void
MTest_setImposedThermodynamicForce2(mtest::MTest& t,
				   const std::string&  n,
				   const std::map<mtest::real,
						  mtest::real>& v)
{
  using namespace std;
  using namespace mtest;
  using mtest::real;
  shared_ptr<Constraint> sc;
  vector<real> tv(v.size());
  vector<real> ev(v.size());
  vector<real>::size_type i;
  map<real,real>::const_iterator pv;
  for(pv=v.begin(),i=0;pv!=v.end();++pv,++i){
    tv[i] = pv->first;
    ev[i] = pv->second;
  }
  shared_ptr<Evolution> sev(new LPIEvolution(tv,ev));
  sc = shared_ptr<Constraint>(new ImposedThermodynamicForce(*(t.getBehaviour()),
								      t.getModellingHypothesis(),
								      n,sev));
  t.addEvolution(n,sev,false,true);
  t.addConstraint(sc);
} // end of MTest_setImposedThermodynamicForce

static void
MTest_setImposedStress(mtest::MTest& t,
		       const std::string&  n,
		       const mtest::real& v)
{
  using namespace std;
  using namespace tfel::material;
  if((t.getBehaviourType()!=MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR)&&
     (t.getBehaviourType()!=MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR)){
    string msg("MTest::handleImposedStress : "
	       "the setImposedStress method is only valid "
	       "for small and finite strain behaviours");
    throw(runtime_error(msg));
  }
  MTest_setImposedThermodynamicForce(t,n,v);
}

static void
MTest_setImposedStress2(mtest::MTest& t,
			const std::string&  n,
			const std::map<mtest::real,
			mtest::real>& v)
{
  using namespace std;
  using namespace tfel::material;
  if((t.getBehaviourType()!=MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR)&&
     (t.getBehaviourType()!=MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR)){
    string msg("MTestParser::handleImposedStress : "
	       "the setImposedStress method is only valid "
	       "for small and finite strain behaviours");
    throw(runtime_error(msg));
  }
  MTest_setImposedThermodynamicForce2(t,n,v);
}

static void
MTest_setImposedCohesiveForce(mtest::MTest& t,
			      const std::string&  n,
			      const mtest::real& v)
{
  using namespace std;
  using namespace tfel::material;
  if(t.getBehaviourType()!=MechanicalBehaviourBase::COHESIVEZONEMODEL){
    string msg("MTestParser::handleImposedCohesiveForce : "
	       "the setImposedCohesiveForce method is only valid "
	       "for small strain behaviours");
    throw(runtime_error(msg));
  }
  MTest_setImposedThermodynamicForce(t,n,v);
}

static void
MTest_setImposedCohesiveForce2(mtest::MTest& t,
			       const std::string&  n,
			       const std::map<mtest::real,
			       mtest::real>& v)
{
  using namespace std;
  using namespace tfel::material;
  if(t.getBehaviourType()!=MechanicalBehaviourBase::COHESIVEZONEMODEL){
    string msg("MTestParser::handleImposedCohesiveForce : "
	       "the setImposedCohesiveForce method is only valid "
	       "for small strain behaviours");
    throw(runtime_error(msg));
  }
  MTest_setImposedThermodynamicForce2(t,n,v);
}

static void
MTest_setImposedDrivingVariable(mtest::MTest& t,
				const std::string&  n,
				const mtest::real& v)
{
  using namespace std;
  using namespace mtest;
  shared_ptr<Constraint> sc;
  shared_ptr<Evolution> sev(new ConstantEvolution(v));
  sc = shared_ptr<Constraint>(new ImposedDrivingVariable(*(t.getBehaviour()),
								   t.getModellingHypothesis(),
								   n,sev));
  t.addEvolution(n,sev,false,true);
  t.addConstraint(sc);
} // end of MTest_setImposedDrivingVariable

static void
MTest_setImposedDrivingVariable2(mtest::MTest& t,
				 const std::string&  n,
				 const std::map<mtest::real,
				 mtest::real>& v)
{
  using namespace std;
  using namespace mtest;
  using mtest::real;
  shared_ptr<Constraint> sc;
  vector<real> tv(v.size());
  vector<real> ev(v.size());
  vector<real>::size_type i;
  map<real,real>::const_iterator pv;
  for(pv=v.begin(),i=0;pv!=v.end();++pv,++i){
    tv[i] = pv->first;
    ev[i] = pv->second;
  }
  shared_ptr<Evolution> sev(new LPIEvolution(tv,ev));
  sc = shared_ptr<Constraint>(new ImposedDrivingVariable(*(t.getBehaviour()),
							 t.getModellingHypothesis(),
							 n,sev));
  t.addEvolution(n,sev,false,true);
  t.addConstraint(sc);
} // end of MTest_setImposedDrivingVariable

static void
MTest_setImposedStrain(mtest::MTest& t,
		       const std::string&  n,
		       const mtest::real& v)
{
  using namespace std;
  using namespace tfel::material;
  if(t.getBehaviourType()!=MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR){
    string msg("MTestParser::handleImposedStrain : "
	       "the setImposedStrain method is only valid "
	       "for small strain behaviours");
    throw(runtime_error(msg));
  }
  MTest_setImposedDrivingVariable(t,n,v);
}

static void
MTest_setImposedStrain2(mtest::MTest& t,
			const std::string&  n,
			const std::map<mtest::real,
			mtest::real>& v)
{
  using namespace std;
  using namespace tfel::material;
  if(t.getBehaviourType()!=MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR){
    string msg("MTestParser::handleImposedStrain : "
	       "the setImposedStrain method is only valid "
	       "for small strain behaviours");
    throw(runtime_error(msg));
  }
  MTest_setImposedDrivingVariable2(t,n,v);
}

static void
MTest_setImposedDeformationGradient(mtest::MTest& t,
		       const std::string&  n,
		       const mtest::real& v)
{
  using namespace std;
  using namespace tfel::material;
  if(t.getBehaviourType()!=MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR){
    string msg("MTestParser::handleImposedDeformationGradient : "
	       "the setImposedDeformationGradient method is only valid "
	       "for finite strain behaviours");
    throw(runtime_error(msg));
  }
  MTest_setImposedDrivingVariable(t,n,v);
}

static void
MTest_setImposedDeformationGradient2(mtest::MTest& t,
			const std::string&  n,
			const std::map<mtest::real,
			mtest::real>& v)
{
  using namespace std;
  using namespace tfel::material;
  if(t.getBehaviourType()!=MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR){
    string msg("MTestParser::handleImposedDeformationGradient : "
	       "the setImposedDeformationGradient method is only valid "
	       "for finite strain behaviours");
    throw(runtime_error(msg));
  }
  MTest_setImposedDrivingVariable2(t,n,v);
}

static void
MTest_setImposedOpeningDisplacement(mtest::MTest& t,
				    const std::string&  n,
				    const mtest::real& v)
{
  using namespace std;
  using namespace tfel::material;
  if(t.getBehaviourType()!=MechanicalBehaviourBase::COHESIVEZONEMODEL){
    string msg("MTestParser::handleImposedOpeningDisplacement : "
	       "the setImposedOpeningDisplacement method is only valid "
	       "for small strain behaviours");
    throw(runtime_error(msg));
  }
  MTest_setImposedDrivingVariable(t,n,v);
}

static void
MTest_setImposedOpeningDisplacement2(mtest::MTest& t,
				     const std::string&  n,
				     const std::map<mtest::real,
				     mtest::real>& v)
{
  using namespace std;
  using namespace tfel::material;
  if(t.getBehaviourType()!=MechanicalBehaviourBase::COHESIVEZONEMODEL){
    string msg("MTestParser::handleImposedOpeningDisplacement : "
	       "the setImposedOpeningDisplacement method is only valid "
	       "for small strain behaviours");
    throw(runtime_error(msg));
  }
  MTest_setImposedDrivingVariable2(t,n,v);
}

static mtest::StudyCurrentState
StudyCurrentState_copy(const mtest::StudyCurrentState& src)
{
  return src;
}

static void
MTest_setRotationMatrix1(mtest::MTest& t,
			 const std::vector<std::vector<mtest::real> >& m,
			 const bool b){
  typedef tfel::math::tmatrix<3u,3u,mtest::real> matrix;
  matrix r;
  for(matrix::size_type i = 0; i!=3;++i){
    for(matrix::size_type j = 0; j!=3;++j){
      r(i,j) = m[i][j];
    }
  }
  t.setRotationMatrix(r,b);
} // end of MTest_setRotationMatrix

static void
MTest_setRotationMatrix2(mtest::MTest& t,
			 const std::vector<std::vector<mtest::real> >& m){
  MTest_setRotationMatrix1(t,m,false);
} // end of MTest_setRotationMatrix

#define TFEL_PYTHON_MTESTCURRENTSTATEGETTER( X )		        \
  static tfel::math::vector<mtest::real>				\
  StudyCurrentState_get##X(const mtest::StudyCurrentState& t)	\
  {                                                                     \
    return t. X ;                                                       \
  }

#define TFEL_PYTHON_MTESTCURRENTSTATEGETTER2( X )		        \
  static mtest::real			                         	\
  StudyCurrentState_get##X(const mtest::StudyCurrentState& t)	\
  {                                                                     \
    return t. X ;                                                       \
  }

TFEL_PYTHON_MTESTCURRENTSTATEGETTER(u_1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER(u0)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER(u1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER2(dt_1)

#define TFEL_PYTHON_MTESTCURRENTSTATEGETTER3( X )	      \
  static tfel::math::vector<mtest::real>		      \
  StudyCurrentState_get##X(const mtest::StudyCurrentState& t) \
  {                                                           \
    const auto& sc = t.getStructureCurrentState("");          \
    if(sc.istates.size()!=1){                                 \
      throw(std::runtime_error("StudyCurrentState::get: "     \
                               "uninitialized state"));       \
    }                                                         \
    return sc. istates[0]. X ;				      \
  }

TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(s_1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(s0)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(s1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(e0)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(e1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(e_th0)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(e_th1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(mprops1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(iv_1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(iv0)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(iv1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(esv0)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(desv)

static mtest::real
StudyCurrentState_getTref(const mtest::StudyCurrentState& t)
{
  const auto& sc = t.getStructureCurrentState("");
  if(sc.istates.size()!=1){
    throw(std::runtime_error("StudyCurrentState::get: "
			     "uninitialized state"));
  }
  return sc. istates[0]. Tref ;
}

void declareMTest(void);

void declareMTest(void)
{
  using namespace boost;
  using namespace boost::python;
  using namespace mtest;
  using boost::python::arg;
  using tfel::tests::TestResult;

  enum_<StiffnessUpdatingPolicy>("StiffnessUpdatingPolicy")
    .value("CONSTANTSTIFFNESS",
	   StiffnessUpdatingPolicy::CONSTANTSTIFFNESS)
    .value("CONSTANTSTIFFNESSBYPERIOD",
	   StiffnessUpdatingPolicy::CONSTANTSTIFFNESSBYPERIOD)
    .value("UPDATEDSTIFFNESSMATRIX",
	   StiffnessUpdatingPolicy::UPDATEDSTIFFNESSMATRIX)
    ;

  enum_<PredictionPolicy>("PredictionPolicy")
    .value("NOPREDICTION",PredictionPolicy::NOPREDICTION)
    .value("LINEARPREDICTION",PredictionPolicy::LINEARPREDICTION)
    .value("ELASTICPREDICTION",PredictionPolicy::ELASTICPREDICTION)
    .value("SECANTOPERATORPREDICTION",
	   PredictionPolicy::SECANTOPERATORPREDICTION)
    .value("TANGENTOPERATORPREDICTION",
	   PredictionPolicy::TANGENTOPERATORPREDICTION)
    ;
  
  enum_<StiffnessMatrixType>("StiffnessMatrixType")
    .value("NOSTIFFNESS",StiffnessMatrixType::NOSTIFFNESS)
    .value("ELASTIC",StiffnessMatrixType::ELASTIC)
    .value("SECANTOPERATOR",StiffnessMatrixType::SECANTOPERATOR)
    .value("TANGENTOPERATOR",StiffnessMatrixType::TANGENTOPERATOR)
    .value("CONSISTENTTANGENTOPERATOR",
	   StiffnessMatrixType::CONSISTENTTANGENTOPERATOR)
    ;

  // for backward compatibility
  enum_<StiffnessMatrixType>("MTestStiffnessMatrixType")
    .value("NOSTIFFNESS",StiffnessMatrixType::NOSTIFFNESS)
    .value("ELASTIC",StiffnessMatrixType::ELASTIC)
    .value("SECANTOPERATOR",StiffnessMatrixType::SECANTOPERATOR)
    .value("TANGENTOPERATOR",StiffnessMatrixType::TANGENTOPERATOR)
    .value("CONSISTENTTANGENTOPERATOR",
	   StiffnessMatrixType::CONSISTENTTANGENTOPERATOR)
    ;
  
  TestResult (MTest:: *pm)(void) = &MTest::execute;
  void (MTest:: *pm2)(StudyCurrentState&,
		      SolverWorkSpace&,
		      const real,
		      const real) const = &MTest::execute;

  class_<StudyCurrentState>("MTestCurrentState")
    .def("copy",&StudyCurrentState_copy)
    .add_property("u_1",StudyCurrentState_getu_1)
    .add_property("u0",StudyCurrentState_getu0)
    .add_property("u1",StudyCurrentState_getu1)
    .add_property("s_1",StudyCurrentState_gets_1)
    .add_property("s0",StudyCurrentState_gets0)
    .add_property("s1",StudyCurrentState_gets1)
    .add_property("e0",StudyCurrentState_gete0)
    .add_property("e1",StudyCurrentState_gete1)
    .add_property("e_th0",StudyCurrentState_gete_th0)
    .add_property("e_th1",StudyCurrentState_gete_th1)
    .add_property("mprops1",StudyCurrentState_getmprops1)
    .add_property("iv_1",StudyCurrentState_getiv_1)
    .add_property("iv0",StudyCurrentState_getiv0)
    .add_property("iv1",StudyCurrentState_getiv1)
    .add_property("evs0",StudyCurrentState_getesv0)
    .add_property("desv",StudyCurrentState_getdesv)
    .add_property("dt_1",StudyCurrentState_getdt_1)
    .add_property("Tref",StudyCurrentState_getTref)
    ;

  class_<SolverWorkSpace,noncopyable>("MTestWorkSpace")
    ;

  class_<MTest,noncopyable>("MTest")
    .def("execute",pm)
    .def("execute",pm2)
    .def("completeInitialisation",
	 &MTest::completeInitialisation)
    .def("initializeCurrentState",
	 &MTest::initializeCurrentState)
    .def("initializeWorkSpace",
	 &MTest::initializeWorkSpace)
    .def("printOutput",
	 &MTest::printOutput)
    .def("setBehaviour",&MTest::setBehaviour)
    .def("setEvolutionValue",
	 &MTest::setEvolutionValue)
    .def("setOutputFileName",
	 &MTest::setOutputFileName)
    .def("setOutputFilePrecision",
	 &MTest::setOutputFilePrecision)
    .def("setResidualFileName",
	 &MTest::setResidualFileName)
    .def("setResidualFilePrecision",
	 &MTest::setResidualFilePrecision)
    .def("setDescription",&MTest::setDescription)
    .def("setAuthor",&MTest::setAuthor)
    .def("setDate",&MTest::setDate)
    .def("setPredictionPolicy",&MTest::setPredictionPolicy)
    .def("setStiffnessMatrixType",
	 &MTest::setStiffnessMatrixType)
    .def("setStiffnessUpdatingPolicy",
	 &MTest::setStiffnessUpdatingPolicy)
    .def("setUseCastemAccelerationAlgorithm",
	 &MTest::setUseCastemAccelerationAlgorithm)
    .def("setCastemAccelerationTrigger",
	 &MTest::setCastemAccelerationTrigger)
    .def("setCastemAccelerationPeriod",
	 &MTest::setCastemAccelerationPeriod)
    .def("setAccelerationAlgorithm",
	 &MTest::setAccelerationAlgorithm)
    .def("setAccelerationParameter",
	 &MTest::setAccelerationAlgorithmParameter)
    .def("setMaximumNumberOfIterations",
	 &MTest::setMaximumNumberOfIterations)
    .def("setMaximumNumberOfSubSteps",
	 &MTest::setMaximumNumberOfSubSteps)
    .def("setDrivingVariableEpsilon",
	 &MTest::setDrivingVariableEpsilon)
    .def("setThermodynamicForceEpsilon",
	 &MTest::setThermodynamicForceEpsilon)
    .def("setParameter",&MTest::setParameter)
    .def("setIntegerParameter",
	 &MTest::setIntegerParameter)
    .def("setUnsignedIntegerParameter",
	 &MTest::setUnsignedIntegerParameter)
    .def("setModellingHypothesis",
	 &MTest::setModellingHypothesis)
    .def("setTimes",&MTest::setTimes)
    .def("addReal",MTest_addEvolution,
	 (arg("name"),"value",arg("b1")=true,arg("b2")=true))
    .def("addEvolution",MTest_addEvolution,
	 (arg("name"),"value",arg("b1")=true,arg("b2")=true))
    .def("addEvolution",MTest_addEvolution2,
	 (arg("name"),"value",arg("b1")=true,arg("b2")=true))
    .def("addEvolution",MTest_addEvolution3,
	 (arg("name"),"value","function",arg("b1")=true,arg("b2")=true))
    .def("setMaterialProperty",MTest_setMaterialProperty,
	 (arg("name"),"value",arg("check")=true))
    .def("setMaterialProperty",MTest_setMaterialProperty2,
	 (arg("name"),"value",arg("check")=true))
    .def("setMaterialProperty",MTest_setMaterialProperty3,
	 (arg("name"),"value","function",arg("check")=true))
    .def("setCastemMaterialProperty",MTest_setCastemMaterialProperty,
	 (arg("name"),arg("library"),arg("function"),arg("check")=true))
    .def("setExternalStateVariable",MTest_setExternalStateVariable,
	 (arg("name"),"value",arg("check")=true))
    .def("setExternalStateVariable",MTest_setExternalStateVariable2,
	 (arg("name"),"value",arg("check")=true))
    .def("setExternalStateVariable",MTest_setExternalStateVariable3,
	 (arg("name"),"value","function",arg("check")=true))
    .def("setImposedStress",MTest_setImposedStress,
	 (arg("name"),"values"))
    .def("setImposedStress",MTest_setImposedStress2,
	 (arg("name"),"values"))
    .def("setImposedCohesiveForce",MTest_setImposedCohesiveForce,
	 (arg("name"),"values"))
    .def("setImposedCohesiveForce",MTest_setImposedCohesiveForce2,
	 (arg("name"),"values"))
    .def("setImposedThermodynamicForce",MTest_setImposedThermodynamicForce,
	 (arg("name"),"values"))
    .def("setImposedThermodynamicForce",MTest_setImposedThermodynamicForce2,
	 (arg("name"),"values"))
    .def("setImposedStrain",MTest_setImposedStrain,
	 (arg("name"),"values"))
    .def("setImposedStrain",MTest_setImposedStrain2,
	 (arg("name"),"values"))
    .def("setImposedDeformationGradient",MTest_setImposedDeformationGradient,
	 (arg("name"),"values"))
    .def("setImposedDeformationGradient",MTest_setImposedDeformationGradient2,
	 (arg("name"),"values"))
    .def("setImposedOpeningDisplacement",MTest_setImposedOpeningDisplacement,
	 (arg("name"),"values"))
    .def("setImposedOpeningDisplacement",MTest_setImposedOpeningDisplacement2,
	 (arg("name"),"values"))
    .def("setImposedDrivingVariable",MTest_setImposedDrivingVariable,
	 (arg("name"),"values"))
    .def("setImposedDrivingVariable",MTest_setImposedDrivingVariable2,
	 (arg("name"),"values"))
    .def("setOutOfBoundsPolicy",
	 &MTest::setOutOfBoundsPolicy)
    .def("setScalarInternalStateVariableInitialValue",
	 &MTest::setScalarInternalStateVariableInitialValue)
    .def("setRotationMatrix",
	 &MTest_setRotationMatrix1)
    .def("setRotationMatrix",
	 &MTest_setRotationMatrix2)
    .def("setScalarInternalStateVariableInitialValue",
	 &MTest::setScalarInternalStateVariableInitialValue)
    .def("setStensorInternalStateVariableInitialValues",
	 &MTest::setStensorInternalStateVariableInitialValues)
    .def("setTensorInternalStateVariableInitialValues",
	 &MTest::setTensorInternalStateVariableInitialValues)
  ;

} // end of declareExternalLibraryManager
