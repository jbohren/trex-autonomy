/* -*- C++ -*-
 * $Id$
 */
/** @file "SimAdapter.cc"
 *
 *
 * @author Frederic Py <fpy@mbari.org>
 */
#include "Utils.hh"
#include "EnumeratedDomain.hh"
#include "IntervalDomain.hh"
#include "BoolDomain.hh"
#include "StringDomain.hh"
#include "SymbolDomain.hh"
#include "Token.hh"
#include "Adapter.hh"
#include "SimAdapter.hh"
#include "Agent.hh"
#include "Utilities.hh"

using namespace TREX;

/*
 * class TREX::SimAdapter
 */

// Statics :


EnumeratedDomain *SimAdapter::xmlAsEnumeratedDomain(TiXmlElement const &elem) {
  std::string typeName;
  SimAdapter::DomainType type = SimAdapter::ANY;

  // Guess the type :
  for(TiXmlElement *child_el = elem.FirstChildElement(); 
      NULL!=child_el; child_el=child_el->NextSiblingElement()) {
    std::string thisType;

    if( strcmp(child_el->Value(), "value")==0 ) 
      thisType = child_el->Attribute("type");
    else
      thisType = child_el->Value();

    checkError(!thisType.empty(), 
	       "SimAdapter:xmlAsEnumeratedDomain error : unable to determine the type of domain.");
    if( "bool"==thisType || "BOOL"==thisType || 
	BoolDomain::getDefaultTypeName().toString()==thisType ) {
      if( ANY==type ) {
	type = BOOL;
	typeName = "bool";
      } 
      if( BOOL==type )
	continue;
    } else if( "int"==thisType || "INT"==thisType || "INT_INTERVAL"==thisType ||
	       IntervalIntDomain::getDefaultTypeName().toString()==thisType ) {
      if( ANY==type ) {
	type = INT;
	typeName = "int";
      } 
      if( INT==type || SimAdapter::FLOAT==type )
	continue;
    } else if( "float"==thisType || "FLOAT"==thisType || "REAL_INTERVAL"==thisType ||
	       IntervalDomain::getDefaultTypeName().toString()==thisType ) {
      if( ANY==type || INT==type ) {
	type = SimAdapter::FLOAT;
	typeName = "float";
      } 
      if( SimAdapter::FLOAT==type )
	continue;
    } else if( "string"==thisType || "STRING"==thisType ||
	       StringDomain::getDefaultTypeName().toString()==thisType ) {
      if( ANY==type ) {
	type = STRING;
	typeName = "string";
      } 
      if( STRING==type )
	continue;
    } else if( "symbol"==thisType ) {
      if( ANY==type ) {
	type = SYMBOL;
	typeName = child_el->Attribute("type");
      } 
      if( SYMBOL==type ) {
	checkError(typeName==child_el->Attribute("type"), 
		   "SimAdapter:xmlAsEnumeratedDomain : symbols from different type in the same enumerated set.");
	continue;
      }
    } else if( "object"==thisType ) {
      checkError(ALWAYS_FAIL, 
		 "SimAdapter:xmlAsEnumeratedDomain : object type is not yet supported.");
    }
    checkError(ALWAYS_FAIL, 
	       "SimAdapter:xmlAsEnumeratedDomain : mixed types in the same enumerated set.");
  }
  checkError(ANY!=type, 
	     "SimAdapter:xmlAsEnumeratedDomain : Unable to guess the set type.");

  // Gather the values :
  std::list<double> values;
  
  for(TiXmlElement *child_el=elem.FirstChildElement();
      NULL!=child_el; child_el=child_el->NextSiblingElement()) {
    char const *value_st = child_el->Attribute("value");
    
    if( NULL==value_st )
      value_st = child_el->Attribute("name");
    checkError(NULL!=value_st,
	       "SimAdapter:xmlAsEnumeratedDomain : unable to extract value.");
    switch( type ) {
    case SimAdapter::FLOAT: 
    case SimAdapter::REAL_INTERVAL: 
    case SimAdapter::BOOL: 
    case SimAdapter::INT: 
    case SimAdapter::INT_INTERVAL: 
    case SimAdapter::STRING: 
    case SimAdapter::SYMBOL:
      values.push_back(getFactory(type).createValue(value_st));
      break;
    case SimAdapter::OBJECT:
    default:
      check_error(ALWAYS_FAIL); // will never happen
    }
  }

  // Return the domain :
  switch( type ) {
  case SimAdapter::BOOL: 
  case SimAdapter::INT: 
  case SimAdapter::INT_INTERVAL: 
  case SimAdapter::FLOAT:
  case SimAdapter::REAL_INTERVAL:
    return new EnumeratedDomain(values, true, typeName.c_str());
  case SimAdapter::STRING:
    return new StringDomain(values, typeName.c_str());
  case SimAdapter::SYMBOL:
    return new SymbolDomain(values, typeName.c_str());
  case SimAdapter::OBJECT:
  default:
    check_error(ALWAYS_FAIL); // will never happen
    return NULL;
  }
} // SimAdapter::xmlAsEnumeratedDomain(TiXmlElement const &)

IntervalDomain *SimAdapter::xmlAsIntervalDomain(TiXmlElement const &elem) {
  char const *min_st = elem.Attribute("min"); 
  char const *max_st = elem.Attribute("max");

  IntervalDomain *domain = dynamic_cast<IntervalDomain *>(m_floatTypeFactory.baseDomain().copy());
  
  checkError(NULL!=domain,
	     "SimAdapter:xmlAsIntervalDomain : type \""<< elem.Attribute("type") <<"\" is not an interval domain type.");

  double min = m_floatTypeFactory.createValue(min_st);
  double max = m_floatTypeFactory.createValue(max_st);
  domain->intersect(min, max);
  return domain;
} // SimAdapter::xmlAsIntervalDomain(TiXmlElement const &)

AbstractDomain *SimAdapter::xmlAsAbstractDomain(TiXmlElement const &elem) {
  std::string tag = elem.Value();
  
  if( "new"==tag ) {
    checkError(ALWAYS_FAIL, 
	       "SimAdapter:xmlAsAbstractDomain : \"new\" object domain is not supported.");
  } else if( "id"==tag ) {
    checkError(ALWAYS_FAIL,
	       "SimAdapter:xmlAsAbstractDomain : \"id\" contrained variable domain is not supported.");
  } else if( "set"==tag ) 
    return xmlAsEnumeratedDomain(elem);
  else if( "interval"==tag )
    return xmlAsIntervalDomain(elem);
  else if( "value"==tag ) {
    char const *type = elem.Attribute("type"); 
    checkError(NULL!=type,
	       "SimAdapter:xmlAsAbstractDomain : missing type for domain.");
    char const *name = elem.Attribute("name");
    checkError(NULL!=name,
	       "SimAdapter:xmlAsAbstractDomain : missing name for domain.");

    AbstractDomain *domain = getFactory(type).baseDomain().copy();
    double val = getFactory(type).createValue(name);
    
    if(domain->isOpen() && !domain->isMember(val))
      domain->insert(val);
    domain->set(val);
    return(domain);
  } else {
    char const *val_st = elem.Attribute("value");
    checkError(NULL!=val_st,
	       "SimAdapter:xmlAsAbstractDomain : missing value for domain.");

    if( "symbol"==tag ) {
      char const *type_st = elem.Attribute("type"); 
      StringDomain*domain = new StringDomain(val_st, type_st);
      /*
      checkError(NULL!=type_st,
		 "SimAdapter:xmlAsAbstractDomain : missing type for domain.");

      AbstractDomain *domain = getFactory(type_st).baseDomain().copy();
      domain->set(getFactory(type_st).createValue(val_st));
      */
      return domain;
    } else {
      checkError("object"!=tag,
		 "SimAdapter:xmlAsAbstractDomain : object parsing not supported.");
    }
  }  
  checkError(ALWAYS_FAIL, 
	     "SimAdapter:xmlAsAbstractDomain : unknown type");
  return NULL;
} // SimAdapter::xmlAsAbstractDomain(TiXmlElement const &)

Observation *SimAdapter::xmlAsObservation(TiXmlElement const &elem) {
  char const *name = elem.Attribute("on");
  checkError(NULL!=name, 
	     "SimAdapter:xmlAsObservation : missing \"on\" attribute");
  char const *pred = elem.Attribute("predicate");
  checkError(NULL!=pred, 
	     "SimAdapter:xmlAsObservation : missing \"predicate\" attribute");
  ObservationByValue *obs = new ObservationByValue(name, pred);
  
  for(TiXmlElement const *i=elem.FirstChildElement(); NULL!=i;
      i = i->NextSiblingElement()) {
    char const *name = i->Attribute("name");
    checkError(NULL!=name,
	       "SimAdapter:xmlAsObservation : missing \"name\" for observation variable.");
    AbstractDomain *dom = xmlAsAbstractDomain(*(i->FirstChildElement()));
    
    obs->push_back(name, dom);
  }

  return obs;
} // SimAdapter::xmlAsObservation(TiXmlElement const &)

// Structors :

SimAdapter::SimAdapter(LabelStr const&agentName, 
		       TiXmlElement const &configData) 
  :TeleoReactor(agentName, configData), m_lastBacktracked(-1),
   m_floatTypeFactory("float"),
   m_intTypeFactory("int"),
   m_boolTypeFactory("bool"),
   m_stringTypeFactory("string"),
   m_symbolTypeFactory("symbol"){
  std::string s = agentName.toString() + ".log";
  std::string file_name = findFile(s);
  TiXmlDocument xml_log(LogManager::use(file_name));

  TREX_INFO("trex:info", "Loading log input file \""<<file_name<<'\"');

  if(!xml_log.LoadFile()){
    TREXLog() << nameString() << "Unable to load xml file \""<<file_name<<'\"';
    std::cerr << nameString() << "Unable to load xml file \""<<file_name<<'\"';
    exit(-1);
  }

  Adapter::getTimelines(m_internals,  Adapter::externalConfig(configData));

  loadObservations(xml_log);
  
} // SimAdapter::SimAdapter

SimAdapter::~SimAdapter() {}

// Modifiers :

void SimAdapter::loadObservations(TiXmlDocument const &file) {
  debugMsg("SimAdapter", "["<<getName().toString()<<"] : loading observation history.");
  TICK maxTick = 0;


  for(TiXmlElement const *tick=firstPath(&file, "Log/Tick")->ToElement();
      NULL!=tick; tick=tick->NextSiblingElement()) 
    if( 0==strcmp(tick->Value(), "Tick") ) {
      std::pair<TICK, Observation *> entry;

      entry.first = strtol(tick->Attribute("value"), NULL, 0);

      if( entry.first>maxTick )
	maxTick = entry.first;

       debugMsg("SimAdapter", "["<<getName().toString()<<"] : observations for tick "<<entry.first);
      for(TiXmlElement const *elem = tick->FirstChildElement();
	  NULL!=elem; elem=elem->NextSiblingElement()) {
	LabelStr timeline = elem->Attribute("on");
	
	if( m_internals.find(timeline)!=m_internals.end() ) {
 	  debugMsg("SimAdapter", "["<<getName().toString()<<"] \t - on <"<<timeline.toString()<<">");

	  entry.second = xmlAsObservation(*elem);
	  m_log.insert(entry);
	}
      }
    }
  debugMsg("SimAdapter", "["<<getName().toString()<<"] : observation history loaded ("<<maxTick<<", "<<m_log.size()<<").");
  m_nextObs = m_log.begin();
}

void SimAdapter::handleInit(TICK initialTick, 
			    std::map<double, ServerId> const &serversByTimeline,
			    ObserverId const &observer) {
  m_observer = observer;
} // SimAdapter::handleInit(TICK, std::map<double, ServerId> const &, ObserverId const &)

bool SimAdapter::synchronize() {
  if( m_log.end()!=m_nextObs ) {
    TICK curTick = getCurrentTick();

    checkError(curTick<=m_nextObs->first, 
	       "SimAdapter:synchronize : playable tick ("<<m_nextObs->first<<") is in the past."); 

    for( ; m_log.end()!=m_nextObs && curTick>=m_nextObs->first; ++m_nextObs ) {
      Observation *obs = m_nextObs->second;
      m_nextObs->second = NULL;
      
      debugMsg("SimAdapter", "["<<getName().toString()<<"]["<<curTick<<"] observation on < "
	       <<obs->getObjectName().toString()<<" >");
   
      m_observer->notify(*obs);
      delete obs;
    }
  }
  else {
    Agent::terminate();
  }

  return true;
} // SimAdapter::synchronize()

// Observers :

void SimAdapter::queryTimelineModes(std::list<LabelStr> &externals, 
				    std::list<LabelStr> &internals) {
  checkError(!m_internals.empty(), 
	     "SimAdapter configuration error for " << getName().toString() 
	     << ". There must be at least one timeline.");
  // Add the standard elements for an adapter
  internals.assign(m_internals.begin(), m_internals.end());
} // SimAdapter::queryTimelineModes(std::list<LabelStr> &, std::list<LabelStr> &)


void SimAdapter::notify(Observation const &obs) {
  debugMsg("SimAdapter:notify", " Ignoring "<<obs.toString());
}

bool SimAdapter::handleRequest(TokenId const &token) {
  debugMsg("SimAdapter:handleRequest", " Ignoring token "<<token->toString());
  return true;
}

void SimAdapter::handleRecall(TokenId const &token) {
  debugMsg("SimAdapter:handleRecall", " Ignoring token"<<token->toString());
}


TypeFactory& SimAdapter::getFactory(SimAdapter::DomainType t){
    switch( t ) {
    case SimAdapter::FLOAT:
    case SimAdapter::REAL_INTERVAL:
      return m_floatTypeFactory;
    case SimAdapter::INT_INTERVAL:
    case SimAdapter::INT: 
      return m_intTypeFactory;
    case SimAdapter::BOOL: 
      return m_boolTypeFactory;
    case SimAdapter::STRING: 
      return m_stringTypeFactory;
    case SimAdapter::SYMBOL:
      return m_symbolTypeFactory;
    default:
      break;
    }

   assertTrue(ALWAYS_FAIL, "No match on input type" + t);
   return m_floatTypeFactory;
}

 TypeFactory& SimAdapter::getFactory(const std::string& t){
   if("float" == t || "REAL_INTERVAL" == t)
     return m_floatTypeFactory;
   if("int" == t || "INT_INTERVAL" == t)
      return m_intTypeFactory;
   if("bool" == t)
      return m_boolTypeFactory;
   if("string" == t)
      return m_stringTypeFactory;
   if("symbol" == t)
      return m_symbolTypeFactory;

   assertTrue(ALWAYS_FAIL, "No match on input type" + t);
   return m_floatTypeFactory;
 }
