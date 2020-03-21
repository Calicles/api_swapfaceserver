utility::string_t to_json(string fileName) {
    std::ifstream InFile( FileName, std::ifstream::binary );
    std::vector<char> data( ( std::istreambuf_iterator<char>( InFile ) ), std::istreambuf_iterator<char>() );

    //std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);
    std::string Code = base64_encode((unsigned char*)&data[0], (unsigned int)data.size());

    string MY_JSON = ("{\"image\" : \"");
    MY_JSON += Code;   // instead of ostrm.str
    MY_JSON += ("\",\"type\" : \"file\"}");

    //set up json object
    json::value obj;
    return obj.parse(utility::conversions::to_string_t(MY_JSON));
}