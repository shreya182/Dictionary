/*
 *Author: Shreya Suresh
 *Description: This program is a dictionary. The interface is the terminal. 
 *The user can get the definition, pronunciation, audio, or example sentence for a word. They can also quit the program.
 */
// TODO File block comment

// TODO If you'd like to include any other header files, do so right below
// this comment


/* BEGINNING OF TEMPLATE CODE; DO NOT MODIFY */




#include <sstream>
#include <iostream>
#include <cstring>
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "cpp-httplib/httplib.h"

#include "json11/json11.hpp"

const std::string DICTIONARY_API_HOST = "https://api.dictionaryapi.dev";
const std::string DICTIONARY_API_PATH_PREFIX = "/api/v2/entries/en/";

using json11::Json;

/*
 * Function: build_request_endpoint
 * Description: Builds the dictionaryapi.dev request endpoint for a given word
 * Parameters:
 * 		word (std::string): Word for which to build request endpoint
 * Returns (std::string): Request endpoint
 */
std::string build_request_endpoint(std::string word) {
	std::stringstream endpoint_ss;
	endpoint_ss << DICTIONARY_API_PATH_PREFIX;
	endpoint_ss << word;
	return endpoint_ss.str();
}

/*
 * Function: extract_definition
 * Description: Extracts the first definition available in the given
 * 		JSON response from dictionaryapi.dev.
 * Parameters:
 * 		words (json11:Json): Parsed json11::Json object of response from
 * 			dictionaryapi.dev
 * Returns (std::string): First definition available in given JSON response.
 * 		If none are available, returns an empty string.
 */
std::string extract_definition(const Json words) {
	// For each word
	const Json::array& word_arr = words.array_items();
	for (const Json& word : word_arr) {
		// Get meanings
		const Json& meanings = word["meanings"];

		// For each meaning
		const Json::array& meaning_arr = meanings.array_items();
		for (const Json& meaning : meaning_arr) {
			// Get thesaurus entries
			const Json thesaurus_entries = meaning["definitions"];

			// For each thesaurus entry
			const Json::array& thesaurus_entry_arr =
				thesaurus_entries.array_items();
			for (const Json& thesaurus_entry : thesaurus_entry_arr) {
				// Get definition
				const std::string definition =
					thesaurus_entry["definition"].string_value();
				
				if (!definition.empty()) {
					// If definition is not empty, return it.
					return definition;
				}

				// Otherwise, keep looking for a non-empty definition.
			}

		}
	}

	// Failed to find definition in response. Return empty string.
	return "";
}

/*
 * Function: query_definition
 * Description: Queries the definition of a given word and returns it
 * 		if the word exists in the dictionaryapi.dev database. Else, it returns
 * 		an empty string.
 * Parameters:
 * 		word (std::string): Word for which to query definition
 * Returns (string): Definition of word if found, or empty string otherwise.
 */
std::string query_definition(std::string word) {
	// Construct http client
	httplib::Client cli(DICTIONARY_API_HOST);

	// Make request
	auto res = cli.Get(build_request_endpoint(word));

	if (res->status != 200) {
		// If status is not 200 (OK), then the word does not exist in the
		// dictionary API's database. Return empty string for definition
		return "";
	}

	// Parse body to Json (array of words)
	std::string err;
	const Json words = Json::parse(res->body, err);
	if (!err.empty()) {
		// If an error occurred in parsing, return an empty string for
		// definition.
		return "";
	}

	// Extract definition
	std::string definition = extract_definition(words);

	return definition;
}

/*
 * Function: extract_audio_url
 * Description: Extracts the first dictionaryapi.dev phonetic audio sample URL
 * 		available from the JSON response.
 * Parameters:
 * 		words (json11:Json): Parsed json11::Json object of response from
 * 			dictionaryapi.dev
 * Returns (std::string): Audio sample URL. If none are available, returns
 * 		an empty string.
 */
std::string extract_audio_url(const Json words) {
	// For each word
	const Json::array& word_arr = words.array_items();
	for (const Json& word : word_arr) {
		// Get phonetics
		const Json phonetics = word["phonetics"];

		// For each phonetic entry
		const Json::array& phonetic_arr = phonetics.array_items();
		for (const Json& phonetic : phonetic_arr) {
			// Get audio url
			const std::string audio_url = phonetic["audio"].string_value();

			if (!audio_url.empty()) {
				// If audio_url is not empty, return it.
				return audio_url;
			}

			// Otherwise, keep looking for a non-empty URL.
		}
	}

	// Failed to find audio URL in response. Return empty string.
	return "";
}

/*
 * Function: query_audio_url
 * Description: Queries the dictionaryapi.dev phonetic audio sample URL of a
 * 		given word and returns it if the word exists in the dictionaryapi.dev
 * 		database. Else, it returns an empty string.
 * Parameters:
 * 		word (std::string): Word for which to query audio sample URL
 * Returns (string): Audio sample URL of word if found. If the word is not found
 * 		in the database or the word doesn't have an audio sample URL associated
 * 		with it, this function returns an empty string.
 */
std::string query_audio_url(std::string word) {
	// Construct http client
	httplib::Client cli(DICTIONARY_API_HOST);

	// Make request
	auto res = cli.Get(build_request_endpoint(word));

	if (res->status != 200) {
		// If status is not 200 (OK), then the word does not exist in the
		// dictionary API's database. Return empty string for definition
		return "";
	}

	// Parse body to Json (array of words)
	std::string err;
	const Json words = Json::parse(res->body, err);
	if (!err.empty()) {
		// If an error occurred in parsing, return an empty string for
		// definition.
		return "";
	}

	// Extract audio URL
	std::string audio_url = extract_audio_url(words);

	return audio_url;
}

/*
 * Function: extract_pronunciation
 * Description: Extracts the first available formal pronunciation from the
 * 		dictionaryapi.dev JSON response.
 * Parameters:
 * 		words (json11:Json): Parsed json11::Json object of response from
 * 			dictionaryapi.dev
 * Returns (std::string): First available pronunciation in JSON response. If
 * 		none are available, returns an empty string.
 */
std::string extract_pronunciation(const Json words) {
	// For each word
	const Json::array& word_arr = words.array_items();
	for (const Json& word : word_arr) {
		// Get pronunciation
		const std::string pronunciation = word["phonetic"].string_value();

		if (!pronunciation.empty()) {
			// If pronunciation is not empty, return it
			return pronunciation;
		}

		// Otherwise, check the ["phonetics"]["text"] fields

		// Get phonetics
		const Json phonetics = word["phonetics"];

		// For each phonetic entry
		const Json::array& phonetic_arr = phonetics.array_items();
		for (const Json& phonetic : phonetic_arr) {
			// Get text of phonetic
			const std::string pronunciation = phonetic["text"].string_value();

			if (!pronunciation.empty()) {
				// If pronunciation is not empty, return it.
				return pronunciation;
			}

			// Otherwise, keep looking for a non-empty pronunciation
		}
	}

	// Failed to find pronunciation in response. Return empty string.
	return "";
}

/*
 * Function: query_pronunciation
 * Description: Queries the dictionaryapi.dev formal pronunciation text of a
 * 		given word and returns it if the word exists in the dictionaryapi.dev
 * 		database. Else, it returns an empty string.
 * Parameters:
 * 		word (std::string): Word for which to query pronunciation
 * Returns (string): Pronunciation of word if found. Else, returns an empty
 * 		string.
 */
std::string query_pronunciation(std::string word) {
	// Construct http client
	httplib::Client cli(DICTIONARY_API_HOST);

	// Make request
	auto res = cli.Get(build_request_endpoint(word));

	if (res->status != 200) {
		// If status is not 200 (OK), then the word does not exist in the
		// dictionary API's database. Return empty string for definition
		return "";
	}

	// Parse body to Json (array of words)
	std::string err;
	const Json words = Json::parse(res->body, err);
	if (!err.empty()) {
		// If an error occurred in parsing, return an empty string for
		// definition.
		return "";
	}

	// Extract pronunciation
	std::string pronunciation = extract_pronunciation(words);

	return pronunciation;
}

/* END OF TEMPLATE CODE */

// TODO Your code goes below this comment.
/*
 * Function: extract_example_sentence
 * Description: Extracts the first example sentence available in the given
 *              JSON response from dictionaryapi.dev.
 * Parameters:
 *              words (json11:Json): Parsed json11::Json object of response from
 *                      dictionaryapi.dev
 * Returns (std::string): First example sentence available in given JSON response.
 *              If none are available, returns an empty string.
 */

std::string extract_example_sentence(const Json words) {
	// For each word
	const Json::array& word_arr = words.array_items();
	for (const Json& word : word_arr) {
		// Get meanings
		const Json& meanings = word["meanings"];

		// For each meaning
		const Json::array& meaning_arr = meanings.array_items();
		for (const Json& meaning : meaning_arr) {
			// Get definitions
			const Json& definitions = meaning["definitions"];

			// For each definition
			const Json::array& definition_arr = definitions.array_items();
			for (const Json& definition : definition_arr) {
				// Get example sentence
				const std::string example = definition["example"].string_value();

				if (!example.empty()) {
					// If example sentence is not empty, return it.
					return example;
				}

				// Otherwise, keep looking for a non-empty example sentence.
			}
		}
	}

	// Return empty string if no example sentence found
	return "";
}
/*
 * Function: query_example_sentence
 * Description: Queries example sentence of a given word and returns it
 *              if the word exists in the dictionaryapi.dev database. Else, it returns
 *              an empty string.
 * Parameters:
 *              word (std::string): Word for which to query example sentence
 * Returns (string): Example sentence for word if found, or empty string otherwise.
 */
std::string query_example_sentence(std::string word) {
        // Construct http client
        httplib::Client cli(DICTIONARY_API_HOST);

        // Make request
        auto res = cli.Get(build_request_endpoint(word));

        if (res->status != 200) {
                // If status is not 200 (OK), then the word does not exist in the
                // dictionary API's database. Return empty string.
                return "";
        }

        // Parse body to Json (array of words)
        std::string err;
        const Json words = Json::parse(res->body, err);
        if (!err.empty()) {
                // If an error occurred in parsing, return empty string.
                return "";
        }

        // Extract example sentence
        std::string example_sentence = extract_example_sentence(words);

        return example_sentence;
}

int main() {	
	bool prompt = true;
	do{ // run dictionary while user doesn't want to quit
	std::string option; 
	//prompt user with available options
	std::cout << "Do you want to lookup the definition of your word(enter 'definition'),"
	       "the audio(enter 'audio'), the pronunciation(enter 'pronunciation'),"
	       "an example sentence(enter 'example'), or exit the program(enter 'exit')" << std::endl;
	std::cin >> option;
	std::string word;
	if (option == "definition"){ // print definition of word
		std::cout << "Enter your word: ";
		std::cin >> word;
		if (query_definition(word) == ""){ // notify user if requested info not found
			std::cout << "No results. The requested information could not be found." << std::endl;
		} else {
			std::cout << "Definition of '" << word << "': " << query_definition(word) << std::endl; 
		}
	} else if (option == "audio"){ // print audio url
		std::cout << "Enter your word: ";
		std::cin >> word;
		if (query_audio_url(word) == ""){ // notify user if requested info not found
			std::cout << "No results. The requested information could not be found." << std::endl;
		} else {
			std::cout << "Audio URL for '" << word << "': " << query_audio_url(word) << std::endl;  
		}
	} else if (option == "pronunciation"){ // print pronunciation of word
		std::cout << "Enter your word: ";
		std::cin >> word;
		if (query_pronunciation(word) == ""){ // notify user if requested info not found
			std::cout << "No results. The requested information could not be found." << std::endl;
		} else {
			std::cout << "Pronunciation of '" << word << "': " << query_pronunciation(word) << std::endl;
		}
	} else if (option == "example"){ // print example sentence of word
		std::cout << "Enter your word: ";
		std::cin >> word;
		if (query_example_sentence(word) == ""){ // notify user if requested info not found
			std::cout << "No results. The requested information could not be found." << std::endl;
		} else {
			std::cout << "Example sentence for '" << word << "': " << query_example_sentence(word) << std::endl; 
		}
	}else if (option == "exit"){ //quit program
		prompt = false;
		return 0;
	} else { //handle invalid input: if user enters invalid input, prompt again
		std::cout << "That is not a valid option! Try again" << std::endl;
	} 
	}while (prompt);

}
