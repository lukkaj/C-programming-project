
// Header for C--programming project


#ifndef PROJECT_H
#define PROJECT_H

typedef struct {
  int gold;
  int silver;
  int bronze;
  char nationName[20];
} Medals;

typedef struct {
  Medals *nations; // pointer to Medals struct
  int size; // amount of nations
} Nation;

#endif
