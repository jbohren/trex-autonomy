/* -*- C++ -*-
 * $Id$
 */
/** @file "EuropaXML.hh"
 * @brief Xml serialization utilities for Europa
 *
 * @author Frederic Py <fpy@mbari.org>
 */
#ifndef _EUROPAXML_HH
# define _EUROPAXML_HH

# include "TREXDefs.hh"

# include "XMLUtils.hh"
# include "AbstractDomain.hh"

# include <typeinfo>


namespace TREX {

  std::string domain_val_to_str(AbstractDomain const &domain, double value, 
				bool symbolic = false);

  //!@{
  /** @brief Utility for Serialization in XML
   *
   * @param Ty the type of the source object
   * @param obj The object to serialize
   *
   * @return The XML structure corresponding to @e obj
   * 
   * @author Frederic Py <fpy@mbari.org>
   */
  template<class Ty>
  TiXmlElement *to_xml(Ty const &obj);


  TiXmlElement *to_xml(AbstractDomain const &domain);
  //!@}
  
  //!@{
  /** @brief Utility for Serialization in XML
   *
   * @param Ty the type of the source object
   * @param out ther output file
   * @param obj The object to serialize
   *
   * Writes the XML form of @e obj
   * 
   * @author Frederic Py <fpy@mbari.org>
   */
//   template<class Ty>
//   void print_xml(FILE *out, Ty const &obj);

  void print_xml(FILE *out, AbstractDomain const &domain);
  //!@}

  
  /** @brief Path extraction in XML
   *
   * @param elem an XML node
   * @param path A Path of tags ala UNIX directory
   *
   * @return The first node corresponding to @e path in @e elem or NULL if @e path does not exist.
   *
   * @author Frederic Py <fpy@mbari.org>
   */
  TiXmlNode const *firstPath(TiXmlNode const *elem, std::string const &path);

} // TREX

#endif // _EUROPAXML_HH
