SECTIONS
{
    .init_array : ALIGN(4)
    {
        _manual_init_array_start = .;
        *(.init_array)
        *(SORT_BY_INIT_PRIORITY(.init_array.*))
        _manual_init_array_end = .;
    }
}
INSERT AFTER .init; /* use INSERT so we don't override default linker script */

SECTIONS
{
    .fini_array : ALIGN(4)
    {
        _manual_fini_array_start = .;
        *(.fini_array)
        *(SORT_BY_INIT_PRIORITY(.fini_array.*))
        _manual_fini_array_end = .;
    }
}
INSERT AFTER .fini; /* use INSERT so we don't override default linker script */

