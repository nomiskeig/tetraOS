.global switch_to_new_page_table

switch_to_new_page_table:
    csrw satp, a0
