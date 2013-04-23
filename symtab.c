#include <stdio.h>
#include <stdlib.h>
#include "vm_value.h"
#include "symtab.h"

struct symbol_table *
symbol_table_create(void)
{
   struct symbol_table *symtab;

   symtab = calloc(1, sizeof(struct symbol_table *));
   if (symtab == NULL) {
      fprintf(stderr, "Memory allocation request failed.\n");
      exit(EXIT_FAILURE);
   }

   symtab->ht_symbols = g_hash_table_new(g_str_hash, g_str_equal);
   return symtab;
}

void
symbol_table_destroy(struct symbol_table *symtab)
{
   g_hash_table_destroy(symtab->ht_symbols);
}

void
symbol_table_put_value(struct symbol_table *symtab,
                       struct vm_value *vmval)
{
   const char *identifier;

   identifier = vmval->identifier;
   g_hash_table_insert(symtab->ht_symbols, (gpointer) identifier, vmval);
}

struct vm_value *
symbol_table_get_value(struct symbol_table *symtab,
                       const char *identifier)
{
   return g_hash_table_lookup(symtab->ht_symbols, identifier);
}


