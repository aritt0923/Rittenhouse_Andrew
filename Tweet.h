#ifndef TWEET_H
#define TWEET_H
#include <string>
#include <vector>
#include <fstream>

class Tweet
{
public:
    /*
    default constructor
    sets tweet_body to empty string
    sets userID to empty string
    sets date to current date
    not intended for use
    */
    Tweet();

    //sets data members to empty string
    void clear();

    /*
    parameterized constructor 
    sets date to current date
    sets userID to passed string (uID)
    sets tweet body to passed string (twt)
    */
    Tweet(std::string uID, std::string twt, std::string dte);

    /*
    returns tweet as a string e.g
    User: (userID)
    Date: (date)
    Tweet: (tweet_body)
    */
    std::string to_tweet();

    std::string to_file(bool for_human);

    std::string get_tweet_date();
    std::string get_tweet_body();

    
private:
std::string date;
std::string tweet_body;
std::string userID;

};
#endif