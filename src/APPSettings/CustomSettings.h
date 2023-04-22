#ifndef CustomSettings_H_INCLUDED
#define CustomSettings_H_INCLUDED
#include <Preferences.h>
using namespace std;
class CustomWebSettings
{
private:
  // Preferences preferences;
public:
   int  SavePreference(const char *key,const char *value, bool override);
  string ReadPreference(const char *key);
    CustomWebSettings(bool start);
    ~CustomWebSettings();
};

#endif