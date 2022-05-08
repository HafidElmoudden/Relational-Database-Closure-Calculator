#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
  char *value;
  int length;
} sinput;

typedef struct
{
  char **value;
  int length;
} msinput;

typedef struct
{
  msinput gauche;
  msinput droite;
  int length;
} DF;

int lengthGetter(char *texte)
{
  int i = 0;
  while (texte[i] != '\0')
    i++;
  return i;
}

sinput inputGetter()
{
  int i = 0, j = 1;
  char *input, character;
  sinput res;
  input = (char *)malloc(sizeof(char));
  while (character != '\n')
  {
    scanf("%c", &character);
    if (character == '\n')
      continue;
    input = (char *)realloc(input, j * sizeof(char));
    input[i] = character;
    i++;
    j++;
  }

  input[i] = '\0';
  res.value = input;
  res.length = i;
  return res;
}

msinput attributesGetter()
{
  char **minput;
  msinput res;
  int i = 0, j = 1, checker = 0;
  sinput input;
  char choice;
  minput = (char **)malloc(sizeof(char *));
  printf("Enter your attributes (type 0 if you want to stop): \n");
  do
  {
    printf("\t ------> Enter the attribute : ");
    input = inputGetter();
    // If you don't break it from here, it will add 0 in the res as well.
    if (input.value[0] == '0' && input.length == 1 && i == 0)
    {
      printf("You haven't entered any attributes!\n");
      continue;
    }
    else if (input.value[0] == '0' && input.length == 1)
      break;
    else if (strcmp(input.value, " ") == 0 || strcmp(input.value, "") == 0)
    {
      printf("You can't enter an empty attribute\n");
      continue;
    }

    for (int x = 0; x < i; x++)
    {
      if (strcmp(input.value, minput[x]) == 0)
      {
        checker = 1;
        break;
      }
    }
    if (checker != 1)
    {
      minput = (char **)realloc(minput, j * sizeof(char *));
      minput[i] = (char *)malloc(input.length * sizeof(char));
      minput[i] = input.value;
      i++;
      j++;
    }
    else
    {
      printf("The attribute is repeated!\n");
    }
    checker = 0;
  } while (3 > 2);

  res.value = minput;
  res.length = i;
  return res;
}

msinput dependancesGetterHelper(char texte[6], msinput attributes)
{
  char **minput;
  msinput res;
  int i = 0, j = 1;
  sinput input;
  minput = (char **)malloc(sizeof(char *));
  int checker = 0, allow;
  printf("Enter your %s attributes (type 0 if you want to stop): \n", texte);
  do
  {
    checker = 0;
    printf("\t ------> Enter the %s attribute : ", texte);
    input = inputGetter();
    // This prevents program from crashing when you dont have any attribute in a specific side, and checks if there's no attribute added in that side.
    if (input.value[0] == '0' && input.length == 1 && i == 0)
    {
      printf("You haven't added anything to the %s side!\n", texte);
      continue;
    }
    // if you dont break it from here it will add 0 in the dependency.
    else if (input.value[0] == '0' && input.length == 1)
      break;
    for (int x = 0; x < attributes.length; x++)
    {
      if (strcmp(input.value, attributes.value[x]) == 0)
      {
        checker = 1;
        break;
      }
    }
    if (checker == 1)
    {
      // This for and allow variable just to prevent repeating attributes in same side.
      for (int y = 0; y < i; y++)
      {
        if (strcmp(input.value, minput[y]) == 0)
        {
          allow = 1;
          break;
        }
        else
          allow = 0;
      }
      if (allow != 1)
      {
        minput = (char **)realloc(minput, j * sizeof(char *));
        minput[i] = (char *)malloc(input.length * sizeof(char));
        minput[i] = input.value;
        i++;
        j++;
      }
      else
      {
        printf("You've already added this attribute!\n");
      }
    }
    else
    {
      printf("The attribute doesn't existe in your relation!\n");
    }
    // You have to reset value of allow from here or it will stuck at value '1'.
    allow = 0;
  } while (1);
  for (int x = 0; x < i; x++)
  {
    printf("---> %s\n", minput[x]);
  }

  res.value = minput;
  res.length = i;
  return res;
}

DF *dependancesGetter(msinput attributes)
{
  int i = 0, j = 1;
  char **input;
  char choice;
  DF *res = malloc(sizeof(DF));

  printf("Enter the dependancies (Enter 0 to stop): \n");
  do
  {
    do
    {
      res = realloc(res, j * sizeof(DF));
      res[i].gauche = dependancesGetterHelper("gauche", attributes);
      if (strcmp(res[i].gauche.value[0], "0") == 0 && res[i].gauche.length == 1)
      {
        break;
      }
      res[i].droite = dependancesGetterHelper("droite", attributes);

      if (strcmp(res[i].droite.value[0], "0") == 0 && res[i].droite.length == 1)
      {
        break;
      }
      i++;
      j++;
      printf("Do you want to add another dependancy ? (Y/n) : ");
      scanf("%c", &choice);
      getchar();
      res->length = i;
      if (choice == 'n' || choice == 'N')
        break;
    } while (1);
    break;
  } while (1);

  return res;
}

msinput closureSetFiller(msinput attributes)
{
  char **minput;
  msinput res;
  int i = 0, j = 1;
  sinput input;
  int checker, counter = 0;
  minput = (char **)malloc(sizeof(char *));
  printf("Enter the names of attributes you want to add to the closure set (type 0 if you want to stop): \n");
  do
  {
    do
    {
      printf("\t ------> Enter the attribute : ");
      input = inputGetter();

      for (int x = 0; x < attributes.length; x++)
      {
        if (strcmp(input.value, attributes.value[x]) == 0)
        {
          checker = 1;
          for (int y = 0; y < i; y++)
          {
            if (strcmp(input.value, minput[y]) == 0)
            {
              printf("The attribute is already in the set!\n");
              checker = 2;
            }
          }
          if (checker == 1)
            counter++;
          break;
        }
        else if (strcmp(input.value, "0") == 0)
        {
          checker = 3;
        }
        else
        {
          checker = 0;
        }
      }
      if (checker == 0)
        printf("The attribute is not found!\n");

    } while (checker != 1 && input.value[0] != '0');
    if (input.value[0] == '0' && i == 0)
    {
      printf("You haven't added any attribute to the set!\n");
      continue;
    }
    // If you don't break it from here, it will add 0 in the res as well.
    else if (input.value[0] == '0')
      break;

    minput = (char **)realloc(minput, j * sizeof(char *));
    minput[i] = (char *)malloc(input.length * sizeof(char));
    minput[i] = input.value;
    i++;
    j++;
  } while (counter < attributes.length);

  res.value = minput;
  res.length = i;
  return res;
}

char **elementAdder(char *element, int elementLength, char **array, int *length)
{
  char **res = array;
  *length += 1;
  res = (char **)realloc(res, *length * sizeof(char *));
  res[*length - 1] = (char *)malloc(elementLength * sizeof(char));
  strcpy(res[*length - 1], element);
  return res;
}

msinput closureCalc(DF *dependances, msinput set)
{
  int allow, check, repeatAllow = 0;
  msinput res;
    // the while(1) loop is for the case where the dependencies are not sorted, so that might mess our closure calc causing it to not be correct. and this does fix that problem!
  while(1){
    repeatAllow = 0;
    for (int i = 0; i < dependances->length; i++)
    {
      for (int j = 0; j < dependances[i].gauche.length; j++)
      {
        for (int z = 0; z < set.length; z++)
        {
          if (strcmp(dependances[i].gauche.value[j], set.value[z]) == 0)
          {
            check = 0;
            break;
          }
          else
          {
            check = 1;
          }
        }
        if (check == 1)
          break;
      }
      if (check != 1)
      {
        for (int y = 0; y < dependances[i].droite.length; y++)
        {
          for (int x = 0; x < set.length; x++)
          {
            // here we are checking if the attribute is already in the closure set
            if (strcmp(dependances[i].droite.value[y], set.value[x]) == 0)
            {
              allow = 1;
              break;
            }
            else
            {
              allow = 0;
              continue;
            }
          }
          if (allow != 1)
          {
            set.value = elementAdder(dependances[i].droite.value[y], lengthGetter(dependances[i].droite.value[y]), set.value, &set.length);
            repeatAllow = 1;
          }
        }
      }
    }
    if(repeatAllow == 0)
      break;
  }
  res = set;
  return res;
}

int main()
{
  msinput attributes;
  sinput relationName;
  DF *dependances;
  msinput closureSet;
  char choice;

  do
  {
    printf("Enter the relation name : ");
    relationName = inputGetter();
    if (strcmp(relationName.value, " ") == 0 || strcmp(relationName.value, "") == 0)
      printf("The relation name is empty!\n");
    else
      break;
  } while (1);
  printf("The relation name is : %s\n", relationName.value);
  attributes = attributesGetter();

  dependances = dependancesGetter(attributes);
  system("cls");
  printf("%s(", relationName.value);
  for (int i = 0; i < attributes.length - 1; i++)
  {
    printf("%s,", attributes.value[i]);
  }
  printf("%s", attributes.value[attributes.length - 1]);
  printf(")\n");

  for (int i = 0; i <= attributes.length - 1; i++)
  {
    printf("Attribute[%d] = %s \n", i + 1, attributes.value[i]);
  }
  for (int x = 0; x < dependances->length; x++)
  {
    printf("\t >> ");
    for (int y = 0; y < dependances[x].gauche.length; y++)
    {
      if (y == dependances[x].gauche.length - 1)
      {
        printf("%s", dependances[x].gauche.value[y]);
        continue;
      }
      printf("%s,", dependances[x].gauche.value[y]);
    }
    printf(" ----> ");
    for (int z = 0; z < dependances[x].droite.length; z++)
    {
      if (z == dependances[x].droite.length - 1)
      {
        printf("%s", dependances[x].droite.value[z]);
        continue;
      }
      printf("%s,", dependances[x].droite.value[z]);
    }
    printf("\n");
  }
  do
  {
    closureSet = closureSetFiller(attributes);

    closureSet = closureCalc(dependances, closureSet);
    printf("{ ");
    for (int i = 0; i < closureSet.length; i++)
    {
      if (i == closureSet.length - 1)
        printf("%s ", closureSet.value[i]);
      else
        printf("%s, ", closureSet.value[i]);
    }
    printf("}\n");
    printf("Do you want to calculate another closure? (Y/n) : \n");
    scanf("%c", &choice);
    getchar();
    if (choice == 'N' || choice == 'n')
      break;
  } while (1);
  return 0;
}