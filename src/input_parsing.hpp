#pragma once

#include "dates.hpp"
#include "parsing.hpp"

#include <string>

const unsigned char YEAR_LENGTH = 6;

/**
 * mm:dd:yyyyyy<a/b>
 * 
 * Sets the value to 0 if the field is empty.
 * Year field is required.
 */
Date parseDate(ReadHead date)
{
    MONTH month = ERR_MONTH;
    unsigned char day = 0;
    bool ad = true;


    // Parse Month
    switch (date.peek()) // First digit of month field
    {
        case '0':
            date.advance(); // Second digit of month field
            switch (date.peek())
            {
                case '1':
                    month = JANUARY;
                    break;
                case '2':
                    month = FEBRUARY;
                    break;
                case '3':
                    month = MARCH;
                    break;
                case '4':
                    month = APRIL;
                    break;
                case '5':
                    month = MAY;
                    break;
                case '6':
                    month = JUNE;
                    break;
                case '7':
                    month = JULY;
                    break;
                case '8':
                    month = AUGUST;
                    break;
                case '9':
                    month = SEPTEMBER;
                    break;
                default:
                    //month = ERR_MONTH; // Month field contained only a '0'
                    break;
            };
            break;
        case '1':
            date.advance(); // Second digit of month field
            switch (date.peek())
            {
                case '0':
                    month = OCTOBER;
                    break;
                case '1':
                    month = NOVEMBER;
                    break;
                case '2':
                    month = DECEMBER;
                    break;
                default:
                    //month = ERR_MONTH; // Month field contained only a '1'
                    break;
            };
            break;
        default:
            //month = ERR_MONTH; // Month field was empty
            break;
    };
    date.advance(); // Colon between month and day

    // Parse Day
    date.advance(); // First digit of day field
    switch (date.peek())
    {
        case '0':
            date.advance(); // Second digit of day field
            break;
        case '1':
            date.advance(); // Second digit of day field
            break;
        default:
            //day = 0; // Day field was empty
            break;
    };
    date.advance(); // Colon between day and year

    // Parse Year
    date.advance(); // First digit of year field
    unsigned int year = stoi(date.read(YEAR_LENGTH));
    date.advance(YEAR_LENGTH); // AD/BC

    // Parse AD/BC
    if (date.peek() == 'b')
    {
        ad = false;
    }

    return Date(year, month, day, ad);
}