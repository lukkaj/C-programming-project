#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "projekti.h"
#include <ctype.h>
#define BUFFERSIZE 80

/*
Function A, add country to database.
In this function country, which is given by the user, is stored to the database.
*/
Nation *addCountry(Nation *data, char *buffer){
  char country_name[20];
  int amount;
  char user_input;

  /*
  Check for too long name, not checking this could lead to stack smashing.
  Buffer is checked up to 23 characters, because first letters do not belong to
  the name of the country which user gives.
  */
  if(strlen(buffer) > 23){
    printf("Name is too long, max length is 20 characters.\n");
    return data;
  }

  /* Input is tested to be in correct form and also test for EOF */
  amount = sscanf(buffer, "%c %s\n", &user_input, country_name);
  if(amount != 2 || amount == EOF){
    printf("Invalid input, back to the main menu.\n");
    return data;
  }

  /* Because country names do not include numbers those are also checked */
  for(int i = 0; i < strlen(country_name); i++){
    if(isdigit(country_name[i]) != 0){
      printf("Country's name must not include numbers.\n");
      return data;
    }
  }

  /* Tester is used to find out if the coutry has been already added. */
  int tester;
  for(int i = 0; i < data->size; i++){
    tester = strcmp(data->nations[i].nationName, country_name);
    if(tester == 0){
      printf("Country has already been added to the database.\n");
      return data;
    }
  }

  /* counter to determine current amount of countries in database. */
  int counter = 0;
  for(int i = 0; i <= (data->size);i++){
    counter = counter + 1;
  }

  /*
  Reallocation of data and also adding country to data->size.
  data->size is the amount of countries in the database.
  */
  data->size = data->size + 1;
  data->nations = realloc(data->nations, sizeof(Medals)*(data->size) + 1);
  if(!data->nations){
    printf("Adding country failed.");
    data->size = data->size - 1;
    return data;
  }

  /*
  Adding countryname to the right index in database. Medals are set to zero.
  */
  strcpy(data->nations[counter-1].nationName, country_name);
  data->nations[counter-1].gold = 0;
  data->nations[counter-1].silver = 0;
  data->nations[counter-1].bronze = 0;

  printf("Country was successfully added to the database.\n");
  return data;
}

/* Function M, Update medals for given country */
void updateCountry(Nation *data, char *buffer){
  char country_name[20];
  char user_input;
  int amount, gld, sil, brz;
  int rightNation = 1;
  int counter = 0;
  int nameCounter = 0;

  /*
  Next few lines are used for checking if name is not over 20 characters.
  First few characters are not included in the name and therefore indexing
  begins from 2.
  */
  for(int i = 2; i < strlen(buffer); i++){
    if(buffer[i] == ' '){
      break;
    }
    nameCounter = nameCounter + 1;
  }
  if(nameCounter > 20){
    printf("Given name is too long, give shorter or an abbreviation.\n");
    return;
  }
  
  /* Check for right input format and EOF. */
  amount = sscanf(buffer, "%c %s %d %d %d", &user_input, country_name,
                                            &gld, &sil, &brz);
  
  /* if not enought inputs or EOF back to main menu */
  if(amount != 5 || amount == EOF){
    printf("Invalid input, back to the main menu.\n");
    return;
  }

  /* Test if coutry is in the database, if not return to main function */
  for(int i = 0; i < (data->size) && rightNation != 0; i++){
    rightNation = strcmp(data->nations[i].nationName ,country_name);
    counter = counter + 1;
  }

  /* If return value of strcmp is not 0, then return back to main menu. */
  if(rightNation != 0){
    printf("No such country in database.\n");
    printf("Countries can be added with command 'A' or read from file 'O'.\n");
    return;
  }

  /*
  Update medals, given amount of medals are added to the database. Negative
  medals are considered douping and the amount is subtracted form the amount of
  certain medal amount.
  */
  data->nations[counter-1].gold = (data->nations[counter-1].gold) + gld;
  data->nations[counter-1].silver = (data->nations[counter-1].silver) + sil;
  data->nations[counter-1].bronze = (data->nations[counter-1].bronze) + brz;
  printf("Medals updated successfully.\n");
  return;
}

/* Function L, print table containing all data. */
void printTable(Nation *data, char *buffer){
  int counter = 0;
  if(data->size == 0){
    printf("Error, empty table, add countries with command 'A' or 'O'.\n");
    return;
  }
  /* If user tries to give too many inputs then back to main menu. */
  if(strlen(buffer) > 2){
    printf("Invalid input, back to main menu.\n");
    return;
  }
  /* Check if user input is correct */
  char user_input;
  int amount = sscanf(buffer, "%c", &user_input);
  if(amount != 1 || amount == EOF){
    printf("Invalid input, back to the main menu.\n");
    return;
  }
  /* How many countries are in database */
  for(int i = 0; i < (data->size);i++){
    counter = counter + 1;
  }
  /*
  Bubble sort to sort nations with most success in the olympics.
  If present value is smaller than next value, then the order will be changed.
  */
  for(int i = 0; i < counter; i++){
    for(int j = i + 1; j < counter; j++){
      if(data->nations[j].gold > data->nations[i].gold){
        Medals temp;
        temp = data->nations[i];
        data->nations[i] = data->nations[j];
        data->nations[j] = temp;
      }
      else if(data->nations[j].gold == data->nations[i].gold &&
        (data->nations[j].silver > data->nations[i].silver)){
        Medals temp;
        temp = data->nations[i];
        data->nations[i] = data->nations[j];
        data->nations[j] = temp;
      }
      else if(data->nations[j].gold == data->nations[i].gold &&
        (data->nations[j].silver == data->nations[i].silver) &&
        (data->nations[j].bronze > data->nations[i].bronze)){
        Medals temp;
        temp = data->nations[i];
        data->nations[i] = data->nations[j];
        data->nations[j] = temp;
      }
    }
  }
  /* print table consisting information from the database */
  printf("\n");
  printf(" ----------------------------------------------- \n");
  for(int i = 0; i < data->size; i++){
      printf("|Nation: %s|  Gold: %d|  Silver: %d|  Bronze: %d|\n",
              data->nations[i].nationName,
              data->nations[i].gold,
              data->nations[i].silver,
              data->nations[i].bronze);
  }
  printf(" ----------------------------------------------- \n");
  return;
}

/* Function W, write to given file. */
void writeFile(Nation *data, char *buffer){
  char user_input;
  char filename[BUFFERSIZE];
  int amount;

  /* Checking if user input is correct */
  amount = sscanf(buffer, "%c %s", &user_input, filename);
  if(amount != 2 || amount == EOF){
    printf("Invalid input, returning back to the main menu.\n");
    return;
  }

  /* Open file in write mode. */
  FILE *fptr = fopen(filename, "w");
  if(!fptr){
    printf("Opening file failed, returning back to main menu.\n");
    return;
  }
  /* fprintf writes information from database to given filename */
  for(int i = 0; i < data->size; i++){
    fprintf(fptr, "%s %d %d %d\n", data->nations[i].nationName,
                                   data->nations[i].gold,
                                   data->nations[i].silver,
                                   data->nations[i].bronze);
  }

  fclose(fptr);
  printf("Data was successfully saved to file %s\n", filename);
  return;
}

/* Function O, Load given file. */
Nation *readFile(Nation *data, char *buffer){
  char user_input;
  char filename[BUFFERSIZE];
  int amount;

  /* Checking if input is correct */
  amount = sscanf(buffer, "%c %s", &user_input, filename);
  if(amount != 2 || amount == EOF){
    printf("Invalid input, returning back to the main menu.\n");
    return data;
  }

  /* Open file in reading mode. */
  FILE *fptr = fopen(filename, "r");
  if(!fptr){
    printf("Opening file failed, returning back to main menu.\n");
    return data;
  }

  /*
  Set data->size (amount of countries) to zero, and overwrite existing database
  with the data from given file line by line.
  */
  char tempLine[BUFFERSIZE];
  int i = 0;
  data->size = 0;
  while(fgets(tempLine, BUFFERSIZE, fptr)){
    char country_name[20];
    int gld;
    int sil;
    int brz;

    /* Parsing file */
    sscanf(tempLine, "%s %d %d %d\n", country_name, &gld, &sil, &brz);
    data->size = data->size + 1;
    data->nations = realloc(data->nations, sizeof(Medals)*(data->size) + 1);
    if(!data->nations){
      printf("Realloc failed, back to main menu.\n");
      return data;
    }

    /* Add file data to the database, copy name and add medals to right index */
    strcpy(data->nations[i].nationName, country_name);
    data->nations[i].gold =  gld;
    data->nations[i].silver = sil;
    data->nations[i].bronze = brz;
    i++;
  }
  if(data->size == 0){
    printf("Read data of %d countries, loaded file is empty.\n", data->size);
  }
  printf("File %s loaded correctly.\n", filename);
  fclose(fptr);
  return data;
}

/* main function */
int main(void){
  Nation data;
  data.nations = malloc(sizeof(Medals));
  memset(data.nations, 0, sizeof(Medals));
  data.size = 0;
  char buffer[BUFFERSIZE];

  printf(" ---------------------------------------------------------\n");
  printf("|               Welcome to Olympics database!             |\n");
  printf(" ---------------------------------------------------------\n");
  printf("\n");
  printf(" ---------------------------------------------------------\n");
  printf("|      Type A, if you want to add country                 |\n");
  printf("|      Type M, if you want to update medals for a country |\n");
  printf("|      Type L, if you want to print medal table           |\n");
  printf("|      Type W, if you want to save your table             |\n");
  printf("|      Type O, if you want to load pre-existing table     |\n");
  printf("|      Type Q, to exit program                            |\n");
  printf("|---------------------------------------------------------|\n");
  printf("|      Example input for A, 'A country name'              |\n");
  printf("|      Example input for M, 'M country name 1 2 3'        |\n");
  printf("|      Example input for L, 'L'                           |\n");
  printf("|      Example input for A, 'W filename'                  |\n");
  printf("|      Example input for A, 'O filename'                  |\n");
  printf("|      Remeber to separate multiple inputs with spaces    |\n");
  printf(" -------------------------------------------------------- \n");
  printf("*Enter command: ");

  /* If reading commands from file and it is empty, then exit the program */
  if(fgets(buffer, sizeof(buffer),stdin) == NULL){
    free(data.nations);
    printf("Thank you for using the program\n");
    return 0;;
  }

  /* If user types Q, then loop will break and the program will shut down */
  while(strcmp(buffer, "Q\n") != 0){
    switch ((int)buffer[0]) {
      case 'A':
        addCountry(&data, buffer);
        break;
      case 'M': ;
        updateCountry(&data, buffer);
        break;
      case 'L':
        printTable(&data, buffer);
        break;
      case 'W':
        writeFile(&data, buffer);
        break;
      case 'O':
        readFile(&data, buffer);
        break;
      default:
        printf("Please enter a correct command.\n");
        break;
    }
    printf("*Enter command: ");
    /* if EOF then break */
    if(fgets(buffer, sizeof(buffer),stdin) == NULL){
      break;
    }
  }

  /* If user input was 'Q' then the loop breaks and all memory is freed. */
  free(data.nations);
  printf("Thank you for using the program.\n");
  return 0;
}
