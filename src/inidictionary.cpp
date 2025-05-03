#include "inidictionary.h"
#include <fstream>
#include <stdlib.h>
#ifdef DEBUG
  #include "SDL.h"
#endif


IniDictionary::IniDictionary(const char* a_pFilename) {
  std::ifstream iniFile(a_pFilename);
  std::string currentSectionName = "";
  std::string inputString;
  do {
    std::getline(iniFile, inputString);
    // [C++11] if (inputString.front() != ';') {
    if (inputString.length() > 0 && inputString[0] != ';') {
      // [C++11] if (inputString.back() == '\r') {
      if (inputString[inputString.length() - 1] == '\r') {
        inputString.erase(inputString.length() - 1);
      }
      trim(inputString);
      if (inputString.length() > 0) {
        // [C++11] if (inputString.front() == '[' && inputString.back() == ']') {
        if (inputString[0] == '[' && inputString[inputString.length() - 1] == ']') {
          // [C++11] inputString.pop_back();
          inputString.erase(inputString.length() - 1);
          std::string sectionName = inputString.substr(1);
          std::map<std::string, std::string> entry;
          std::pair<std::string, std::map<std::string, std::string> > section(sectionName, entry);
#ifdef DEBUG
          printf("IniDictionary: section found: %s\n", sectionName.c_str());
#endif
          m_dictionary.insert(section);
          currentSectionName = sectionName;
        }
        else {
          int equalsSignPos = inputString.find_first_of('=');
          std::string key = inputString.substr(0, equalsSignPos);
          trim(key);
          std::string value = inputString.substr(equalsSignPos + 1);
          trim(value);
          std::pair<std::string, std::string> entry(key, value);
#ifdef DEBUG
          printf("IniDictionary: pair found: %s = %s\n", key.c_str(), value.c_str());
#endif
          m_dictionary[currentSectionName].insert(entry);
        }
      }
    }
  }
  while (!iniFile.eof());
}

void IniDictionary::trim(std::string& a_string) {
  // [C++11] while (a_string.back() == ' ') {
  while (a_string.length() > 0 && a_string[a_string.length() - 1] == ' ') {
    // [C++11] a_string.pop_back();
    a_string.erase(a_string.length() - 1);
  }
  // [C++11] if (a_string.front() == ' ') {
  if (a_string[0] == ' ') {
    int nonSpacePos = a_string.find_first_not_of(' ');
    a_string.erase(0, nonSpacePos);
  }
}

bool IniDictionary::getBoolValue(const char* a_pKey, const char* a_pSectionName, bool a_defaultValue) {
#ifdef DEBUG
  printf("getBoolValue: key %s from section %s\n", a_pKey, a_pSectionName);
#endif
  int nSectionKeys = m_dictionary.count(a_pSectionName);
  if (nSectionKeys == 0) {
#ifdef DEBUG
    printf("getBoolValue: WARNING: section not found\n");
#endif
    return a_defaultValue;
  }
  int nKeys = m_dictionary[a_pSectionName].count(a_pKey);
  if (nKeys == 0) {
#ifdef DEBUG
    printf("getBoolValue: WARNING: key not found\n");
#endif
    return a_defaultValue;
  }
  std::string value = m_dictionary[a_pSectionName][a_pKey];
  if (value == "true") {
    return true;
  }
  if (value == "false") {
    return false;
  }
  return a_defaultValue;
}

const char* IniDictionary::getCStringValue(const char* a_pKey, const char* a_pSectionName, const char* a_defaultValue) {
#ifdef DEBUG
  printf("getCStringValue: key %s from section %s\n", a_pKey, a_pSectionName);
#endif
  int nSectionKeys = m_dictionary.count(a_pSectionName);
  if (nSectionKeys == 0) {
#ifdef DEBUG
    printf("getCStringValue: WARNING: section not found\n");
#endif
    return a_defaultValue;
  }
  int nKeys = m_dictionary[a_pSectionName].count(a_pKey);
  if (nKeys == 0) {
#ifdef DEBUG
    printf("getCStringValue: WARNING: key not found\n");
#endif
    return a_defaultValue;
  }
  std::string value = m_dictionary[a_pSectionName][a_pKey];
  return value.c_str();
}

int IniDictionary::getIntValue(const char* a_pKey, const char* a_pSectionName, int a_defaultValue) {
#ifdef DEBUG
  printf("getIntValue: key %s from section %s\n", a_pKey, a_pSectionName);
#endif
  int nSectionKeys = m_dictionary.count(a_pSectionName);
  if (nSectionKeys == 0) {
#ifdef DEBUG
    printf("getIntValue: WARNING: section not found\n");
#endif
    return a_defaultValue;
  }
  int nKeys = m_dictionary[a_pSectionName].count(a_pKey);
  if (nKeys == 0) {
#ifdef DEBUG
    printf("getIntValue: WARNING: key not found\n");
#endif
    return a_defaultValue;
  }
  int intValue = atoi(m_dictionary[a_pSectionName][a_pKey].c_str());
  return intValue;
}
