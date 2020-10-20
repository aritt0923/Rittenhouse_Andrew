#include <iostream>
#include "Tweet.h"
#include <string>

   
    //default constructor
    Tweet::Tweet()
    {
       clear(); 
    }
    
    //parameterized constructor 
    Tweet::Tweet(std::string uID, std::string twt, std::string dte)
    {
        userID = uID;
        tweet_body = twt;
        date = dte;
    }

    //sets data members to empty string
    void Tweet::clear()
    {
        userID = "";
        tweet_body = "";
        date = "";          
    }

    std::string Tweet::get_tweet_body()
    {
        return tweet_body;
    }

    std::string Tweet::get_tweet_date()
    {
        return date;
    }

    
    //returns tweet as a string
    std::string Tweet::to_tweet()
    {
        std::string str;
        str = "Date: " + date + '\n' + "Username: " + userID + '\n' + tweet_body + '\n'; 
        return str; 
    }

    std::string Tweet::to_file(bool for_human)
    {
        std::string str;
        if(for_human)
        {
            str = "Date: " + date + '\n' + "Username: " + userID + '\n' + tweet_body + '\n';
            return str;
        }
        str = '*' + date + "*" + '\n' + '*' + userID + "*" + '\n' + '*' + tweet_body + "*" + '$' + '\n'; 
        return str; 
    }

