#ifndef API_HELPERS_H
#define API_HELPERS_H

#include "structures.h"
#include "json.hpp"
#include "json_fwd.hpp"
#include <cstddef>
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"


#include <string>

std::string getToken(std::string respBody) {
    nlohmann::json resp = nlohmann::json::parse(respBody);
    return resp["accountInfo"]["singleSignToken"];
}

std::string getBookshelf (std::string token) {
    std::string url_bookshelf = "/api/getBookShelf.action?token="+token;
    httplib::Client cli("https://www.storytel.com");
    auto res = cli.Get(url_bookshelf);
    if (res->status!=200) return "FAILED";
    return res->body;
}

std::string getStreamUrl(uint64_t id, std::string token) {
    std::string streamUrl = "/mp3streamRangeReq?startposition=0&programId="+std::to_string(id)+"&token="+token;
    httplib::Client cli("https://www.storytel.com");
    auto res = cli.Get(streamUrl);
    return res->body;
}

void setBookmark(std::string token, std::string bookId, int64_t pos, int64_t curBookmark) {
    httplib::Client cli("https://www.storytel.se");
    std::string urlBookmark = "/api/setABookmark.action?token="+token+"&bookId="+bookId+"&pos="+std::to_string(curBookmark + pos)+"&type=1";
    auto res = cli.Get(urlBookmark);
}

std::vector<BookEntry> parseEntries(std::string bookshelf) {
    nlohmann::json books = nlohmann::json::parse(bookshelf);

    std::vector<BookEntry> bookEntries;

    for (auto it : books["books"]) {
        if(!it.at("abook").is_null() && !it.at("abookMark").is_null()){
            Book p;
            ABook abook;
            BookMark bookmark;
            it.at("book").at("name").get_to(p.title);
            it.at("book").at("authorsAsString").get_to(p.author);
            it.at("abook").at("id").get_to(abook.id);
            it.at("abookMark").at("pos").get_to(bookmark.position);
            it.at("abookMark").at("bookId").get_to(bookmark.id);


            BookEntry be{abook, bookmark, p};
            bookEntries.push_back(be);
        }
    }
    return bookEntries;
}

#endif // API_HELPERS_H
