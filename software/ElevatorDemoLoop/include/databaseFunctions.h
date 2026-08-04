#ifndef DB_FUNCTIONS

#define DB_FUNCTIONS
int db_getFloorNum();
int db_setFloorNum(int floorNum);
bool db_getNextCommand(int &canID, int &data);
int db_addCANMessage(int canID, int data);
int db_addDiagnostic(int nodeID, const std::string &message);

#endif
