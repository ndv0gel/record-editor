#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_RECORDS 1000
#define NAME_LEN 64
#define OWNER_LEN 64

typedef struct {
  int id;
  char name[NAME_LEN];
  int quantity;
  char owner[OWNER_LEN];
} Record;

static Record records[MAX_RECORDS];
static int count = 0;
static int next_id = 1;

static void trim_newline(char *s){
  size_t n = strlen(s);
  if(n && (s[n-1]=='\n' || s[n-1]=='\r')) s[n-1] = '\0';
}

static void load_csv(){
  FILE *f = fopen("records.csv", "r");
  if(!f) return;
  char line[256];
  count = 0; next_id = 1;
  while(fgets(line, sizeof line, f)){
    trim_newline(line);
    if(!*line) continue;
    Record r = {0};
    char *tok;
    tok = strtok(line, ","); if(!tok) continue; r.id = atoi(tok);
    tok = strtok(NULL, ","); if(!tok) continue; strncpy(r.name, tok, NAME_LEN-1);
    tok = strtok(NULL, ","); if(!tok) continue; r.quantity = atoi(tok);
    tok = strtok(NULL, ","); if(!tok) continue; strncpy(r.owner, tok, OWNER_LEN-1);
    if(count < MAX_RECORDS){ records[count++] = r; if(r.id >= next_id) next_id = r.id + 1; }
  }
  fclose(f);
}

static void save_csv(){
  FILE *f = fopen("records.csv", "w");
  if(!f){ perror("records.csv"); return; }
  for(int i=0;i<count;i++){
    fprintf(f, "%d,%s,%d,%s\n", records[i].id, records[i].name, records[i].quantity, records[i].owner);
  }
  fclose(f);
}

static void add_record(){
  if(count >= MAX_RECORDS){ puts("Storage full"); return; }
  Record r = {0};
  r.id = next_id++;
  printf("Name: ");
  if(!fgets(r.name, sizeof r.name, stdin)) return; trim_newline(r.name);
  printf("Quantity: ");
  char buf[32]; if(!fgets(buf, sizeof buf, stdin)) return; r.quantity = atoi(buf);
  printf("Owner: ");
  if(!fgets(r.owner, sizeof r.owner, stdin)) return; trim_newline(r.owner);
  records[count++] = r; save_csv();
  printf("Added id=%d\n", r.id);
}

static void list_records(){
  if(!count){ puts("No records."); return; }
  puts("ID  | NAME                           | QTY | OWNER");
  puts("----+--------------------------------+-----+------------------------------");
  for(int i=0;i<count;i++){
    printf("%-3d | %-30s | %-3d | %-28s\n", records[i].id, records[i].name, records[i].quantity, records[i].owner);
  }
}

static Record* find_by_id(int id){
  for(int i=0;i<count;i++) if(records[i].id==id) return &records[i];
  return NULL;
}

static void search_by_id(){
  printf("ID: ");
  char buf[32]; if(!fgets(buf, sizeof buf, stdin)) return; int id = atoi(buf);
  Record* r = find_by_id(id);
  if(!r){ puts("Not found."); return; }
  printf("Found: id=%d name=%s qty=%d owner=%s\n", r->id, r->name, r->quantity, r->owner);
}

static void search_by_name(){
  printf("Name contains: ");
  char needle[NAME_LEN]; if(!fgets(needle, sizeof needle, stdin)) return; trim_newline(needle);
  if(!*needle){ puts("Empty query."); return; }
  int hits = 0;
  for(int i=0;i<count;i++){
    if(strstr(records[i].name, needle)){
      printf("%-3d | %-30s | %-3d | %-28s\n", records[i].id, records[i].name, records[i].quantity, records[i].owner);
      hits++;
    }
  }
  if(!hits) puts("No matches.");
}

static void delete_by_id(){
  printf("ID: ");
  char buf[32]; if(!fgets(buf, sizeof buf, stdin)) return; int id = atoi(buf);
  int idx = -1; for(int i=0;i<count;i++) if(records[i].id==id){ idx=i; break; }
  if(idx<0){ puts("Not found."); return; }
  for(int j=idx;j<count-1;j++) records[j]=records[j+1];
  count--; save_csv();
  puts("Deleted.");
}

static void menu(){
  puts("\nRecord Editor\n--------------");
  puts("1) Add record");
  puts("2) List all");
  puts("3) Search by id");
  puts("4) Search by name");
  puts("5) Delete by id");
  puts("0) Quit");
  printf("> ");
}

int main(){
  load_csv();
  char line[32];
  for(;;){
    menu();
    if(!fgets(line, sizeof line, stdin)) break;
    int choice = atoi(line);
    switch(choice){
      case 1: add_record(); break;
      case 2: list_records(); break;
      case 3: search_by_id(); break;
      case 4: search_by_name(); break;
      case 5: delete_by_id(); break;
      case 0: puts("Bye."); return 0;
      default: puts("Invalid option.");
    }
  }
  return 0;
}
