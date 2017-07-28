#include "ibutton.h"
//#include "TOUCH.C"    // from CCS C compiler library
//#include "LCD.C"      // from CCS C compiler library

#define eeprom_byte 16
#define wait_time 80
#define switch1 pin_a0
#define switch2 pin_a1
#define led_red pin_a2
#define led_green pin_a3

BYTE Delete[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

void eeprom_write(unsigned char address, unsigned char buffer[8])
{
    unsigned char i;

    for (i = 8; i > 0; i--)
    {
        write_eeprom(address++, buffer[i - 1]);
    }
}

char eeprom_read(unsigned char address)
{
    unsigned char i;
    unsigned char readbyte;

    lcd_putc("\f");
    printf(lcd_putc, "%X", address);
    lcd_putc(":");
    for (i = 3; i < 7; i++)
    {
        readbyte = read_eeprom(8 * address + i);
        printf(lcd_putc, "%X", readbyte);
    }
}

int1 eeprom_compare(unsigned char numara[8])
{
    unsigned char i, k, reg = 0;
    int result = 0;

    for (i = 0; i < eeprom_byte; i++)
    {
        if (
            read_eeprom(reg) == numara[7] &
            read_eeprom(reg + 1) == numara[6] &
            read_eeprom(reg + 2) == numara[5] &
            read_eeprom(reg + 3) == numara[4] &
            read_eeprom(reg + 4) == numara[3] &
            read_eeprom(reg + 5) == numara[2] &
            read_eeprom(reg + 6) == numara[1] &
            read_eeprom(reg + 7) == numara[0])
        {
            if (reg == 0)
            {
                result = 2;
                break;
            }
            else
            {
                result = 1;
                break;
            }
        }
        reg += 8;
    }

    return result;
}

char prepare_data_space(void)
{
    unsigned char i, reg, test;
    int result;

    reg = 0;
    result = 8;

    for (i = 0; i < eeprom_byte; i++)
    {

        test = !read_eeprom(reg) | !read_eeprom(reg + 1) | !read_eeprom(reg + 2) | !read_eeprom(reg + 3) | !read_eeprom(reg + 4) | !read_eeprom(reg + 5) | !read_eeprom(reg + 6) | !read_eeprom(reg + 7);
        if (test == 0)
        {
            if (reg == 0)
            {
                result = 9;
                break;
            }
            else
            {
                result = i;
                break;
            }
        }
        reg += 8;
    }

    return result;
}

void green(void)
{
    lcd_putc("\fTOM OK.");
    output_bit(led_green, 1);
    delay_ms(1000);
    output_bit(led_green, 0);
    reset_cpu();
}

void red(void)
{
    lcd_putc("\fTOM INVALID!");
    output_bit(led_red, 1);
    delay_ms(1000);
    output_bit(led_red, 0);
    reset_cpu();
}

int wait_selection(void)
{
    int i, selection, b1, b2;
    selection = 0;
    for (i = 0; i < wait_time; i++)
    {
        delay_ms(100);
        b1 = input(switch1);
        b2 = input(switch2);
        if (b1 | b2)
        {
            if (b1)
            {
                for (;;)
                {
                    if (!input(switch1))
                    {
                        selection = 1;
                        break;
                    }
                }
            }
            if (b2)
            {
                for (;;)
                {
                    if (!input(switch2))
                    {
                        selection = 2;
                        break;
                    }
                }
            }
            break;
        }
    }

    return selection;
}

char read_button(char buffer[8], char presenceCheck)
{
    char i;
    while (!touch_present())
        ;
    delay_ms(200);
    if (touch_present())
    {
        touch_write_byte(0x33);
        for (i = 0; i < 8; ++i)
            buffer[i] = touch_read_byte();
        delay_ms(1000);

        presenceCheck = buffer[0] & buffer[1] & buffer[2] & buffer[3] & buffer[4] & buffer[5] & buffer[6] & buffer[7];
        if (buffer[6] = 0)
            presenceCheck = 0xFF;
        return buffer;
        return presenceCheck;
    }
}

void main()
{
    unsigned CHAR buffer[8];
    int i, selection, selection2, k, l;
    unsigned CHAR presenceCheck, temp, master_absent, space;

    port_b_pullups(TRUE);
    setup_timer_0(RTCC_INTERNAL);
    setup_timer_1(T1_DISABLED);
    setup_timer_2(T2_DISABLED, 0, 1);
    setup_ccp1(CCP_OFF);
    setup_comparator(NC_NC_NC_NC);
    setup_vref(FALSE);
    set_tris_b(0x08);
    lcd_init();

    lcd_putc("\fREADY.");
    master_absent = 0;
    if (prepare_data_space() == 0x09)
    {
        lcd_putc("\fMaster TOM Registration\nTouch the TOM");
        master_absent = 1;
    }

    while (TRUE)
    {

        read_button(buffer, presenceCheck);

        if (presenceCheck != 0xFF) // iButton has been touched
        {

            if (master_absent)
            {
                eeprom_write(0, buffer);
                lcd_putc("\fTouch again\nfor verification");
                read_button(buffer, presenceCheck);
                for (i = 0; i < wait_time; i++)
                {
                    delay_ms(100);
                    if (presenceCheck != 0xFF)
                    {
                        k = eeprom_compare(buffer);
                        if (k == 2)
                        {
                            lcd_putc("\fRegistration OK");
                            master_absent = 0;
                            for (i = 0; i < 10; i++)
                                delay_ms(100);
                            reset_cpu();
                        }
                        else
                        {
                            lcd_putc("\fNot Approved");
                            for (i = 0; i < 8; i++)
                                write_eeprom(i, 0xFF);
                            for (i = 0; i < 10; i++)
                                delay_ms(100);
                            reset_cpu();
                        }
                    }
                }
                reset_cpu();
            }

            if (eeprom_compare(buffer) == 1 || eeprom_compare(buffer) == 2)
            {
                if (eeprom_compare(buffer) == 2)
                {
                    //Master is authorized
                    lcd_putc("\fMaster Authorized\n<-Options  Door->");
                    selection = wait_selection();
                    switch (selection)
                    {
                    case 0:
                        //Selection is not made
                        reset_cpu();
                        break;

                    case 1:
                    //MASTER Choose "Options"
                    process:
                        lcd_putc("\fChoose Operation\n<-Add     Remove->");
                        selection2 = wait_selection();
                        switch (selection2)
                        {
                        case 0:
                            reset_cpu();
                            break;
                        case 1:
                            //MASTER choose "Add"
                            space = prepare_data_space();
                            if (space == 0x08)
                            { // Not enough space
                                lcd_putc("Not enough space!");
                                delay_ms(1000);
                                reset_cpu();
                            }
                            if (space == 0x09)
                                reset_cpu(); // Master is not authorized
                        registration:
                            lcd_putc("\fYeni TOM'u\ndokundurun"); // Other state
                            for (i = 0; i < wait_time; i++)
                            {
                                delay_ms(100);
                                read_button(buffer, presenceCheck);
                                if (presenceCheck != 0xFF)
                                {
                                    if (eeprom_compare(buffer) != 0)
                                    {
                                        lcd_putc("\fAlready registered.");
                                        for (i = 0; i < 2; i++)
                                            delay_ms(1000);
                                        goto registration;
                                    }
                                    else
                                    {
                                        eeprom_write(space * 0x08, buffer);
                                        lcd_putc("\fStored.");
                                        for (i = 0; i < 2; i++)
                                            delay_ms(1000);
                                        goto islem;
                                    }
                                }
                            }
                            break;
                        case 2:
                            //MASTER selected "Remove"
                            k = 0;
                        delete:
                            eeprom_read(k);
                            lcd_putc("\n<-Next  Delete->");
                            for (i = 0; i < wait_time; i++)
                            {
                                delay_ms(100);
                                if (input(switch1))
                                {
                                    for (l = 0; l < wait_time; l++)
                                    {
                                        delay_ms(100);
                                        if (!input(switch1))
                                            goto increase;
                                    }
                                    reset_cpu();
                                increase:
                                    if (k == eeprom_byte - 1)
                                    {
                                        k = 0;
                                    }
                                    else
                                    {
                                        k += 1;
                                    }
                                    goto delete;
                                }
                                if (input(switch2))
                                {
                                    for (l = 0; l < wait_time; l++)
                                    {
                                        delay_ms(100);
                                        if (!input(switch2))
                                        {
                                            //silme i�lemi yap
                                            printf(lcd_putc, "\f%X Are you sure?\n<-Yes   No->", k);
                                            selection2 = wait_selection();
                                            switch (selection2)
                                            {
                                            case 1:
                                                eeprom_write(k * 0x08, Sil);
                                                lcd_putc("\fDeleted.");
                                                delay_ms(1000);
                                                if (k == 0)
                                                    reset_cpu();
                                                else
                                                    goto process;
                                                break;
                                            case 2:
                                                goto sil;
                                                break;
                                            }
                                            delay_ms(1000);
                                        }
                                    }
                                }
                            }
                            reset_cpu();

                            break;
                        }
                        break;
                    case 2:
                        //MASTER Chose "Door", Open the door
                        green();
                        break;
                    }
                }
                else
                {
                    // Open the door
                    green();
                }
            }
            else
            {
                // Not registered
                red();
            }
        }
    }
}
