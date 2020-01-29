//
// Created by jor-dro on 1/29/20.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "graph.h"
#include "ops.h"
#include "hashmap.h"

void user() {
  int input; char *name_db, path_db[MAX_STRING];
  ERROR_CODE error = NO_ERROR;
  printf("1. Create DB \n2. Read DB \n3. Exit \n");
  scanf("%d", &input);
  switch(input) {
    case 1: {
      printf("\nPlease enter name of DB: ");
      scanf("%s", name_db);
      printf("\nInteraction with graph %s \n", name_db);
      printf("1. Vertex \n2. Edge \n3. Set graph property \n4. Exit \n");
      int input = 0;
      scanf("%d", &input);
      char *path = (char*) malloc(MAX_STRING*sizeof(char));
      sprintf(path, "%s.graphml", name_db);
      graph *g = create_graph(name_db, path, error);
      switch(input) {
        case 1:
        printf("\n1. Create Vertex \n2. Update Vertex \n3. Delete Vertex\n4. Find Vertex \n5. Exit \n");
        int input = 0;
        scanf("%d", &input);
        switch(input) {
          case 1:
            printf("No action here yet!\n");
            break;
          case 2:
            printf("No action here yet!\n");
            break;
          case 3:
            printf("No action here yet!\n");
            break;
          case 4:
          printf("\n1. Per path \n2. Per ID \n3. Per property \n4. Exit \n");
          break;
              int input = 0;
              scanf("%d", &input);
              switch(input) {
                case 1:
                  printf("\nStart ID \n");
                  break;
                  uint64_t id;
                  scanf("%lld", &id);
                  printf("\nInput: %lld", id);
                  int input = 0;
                  scanf("%d", &input);
                  printf("\nFind destination \n1. per ID \n2. per property \n3. per label \n4. Exit \n");
                        switch(input) {
                          case 1:
                            printf("1. Depth Search \n2. Breadth Search \n");
                            //add scanf and switch case here
                          case 2:
                            printf("1. Depth Search \n2. Breadth Search \n");
                            //add scanf and switch case here
                          case 3:
                            printf("1. Depth Search \n2. Breadth Search \n");
                            //add scanf and switch case here
                          case 4:
                            break;
                          default:
                            printf("Please choose the menu by entering the number! \n");
                            break;
                            }
               case 2:
                printf("Please enter ID: ");
                //uint64_t id;
                //scanf("%d", &id);
                //printf("Input: %d", &id);
              case 3:
                printf("Please enter name of property: ");
                //add scanf for property here.
                printf("Please enter type of property: ");
                //add scanf for property_t here
                printf("Please enter value of property: ");
                //add scanf for value here
              case 4:
                break;
              default:
                printf("Please choose the menu by entering the number! \n");
                break;
              }
          case 5:
            break;
          default:
            printf("Please choose the menu by entering the number! \n");
            break;
          }
          break;
            case 2: printf("1. Create Edge \n2. Update Edge \n3. Delete Edge \n4. Find Edge \n5. Exit \n"); break;
            case 3: printf("No action here yet!"); break;
            case 4: break;
            default: printf("Please choose the menu by entering the number! \n"); break;
          }
        }
        break;
    case 2: printf("Please enter path of DB: ");
            scanf("%s", path_db);
            printf("\nPlease enter name of DB: ");
            scanf("%s", name_db);
            //printf("Ihre Eingabe: %s und %s ", path_db, name_db);
            break;
    case 3: break;
    default: printf("\nPlease choose the menu by entering the number! \n");
  }
}
