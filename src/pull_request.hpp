#pragma once

#include "horizons/horizons.hpp"
#include "horizons/horizon_output_parsing.hpp"
#include "cache/cache.hpp"
#include "cache/filenames.hpp"
#include "cache/cache_file.hpp"
#include "dates.hpp"
#include "debugging.hpp"

#include <string>

typedef struct {
    std::string object;
    Date date;
} Query;

// Returns the path to the file that was pulled
std::string pullDay(Query* query, const CacheMeta& cache_meta)
{
    DEBUG_PRINT("\tEntered Pull Request for " << query->object << " on " << query->date.dayRequestStartString());
    DaySaveFilePath day_file_path(DayFileID{query->object, query->date});
    std::string output_file_path = day_file_path.getPath();

    DEBUG_PRINT("\t\tLooking for output_file_path " << output_file_path << " in cache_meta")
    for (auto& cache_file : cache_meta.getCacheFiles())
    {
        DEBUG_PRINT("\t\t\tChecking Cache File: " << cache_file.getFileName());
        if ("cache/" + cache_file.getFileName() == output_file_path)
        {
            return output_file_path;
        }
    }

    HorizonsPull pull(query->object, query->date.dayRequestStartString(), query->date.dayRequestEndString(), "1m");
    pull.makeRequest(output_file_path + ".raw");

    DEBUG_PRINT("\tOpening Horizon Output File")
    HorizonOutputFile opf(output_file_path + ".raw");
    DEBUG_PRINT("\tIsolating Horizon Generated Ephemeris")
    HorizonGeneratedEphemeris ephem(opf);
    DEBUG_PRINT("\tParsing Horizon Generated Ephemeris")
    ParsedHorizonGeneratedEphemeris pephem(ephem);

    writeCacheFile(pephem, output_file_path);

    return output_file_path;
}

std::string pullYear(Query* query, const CacheMeta& cache_meta)
{
    YearSaveFilePath year_file_path(YearFileID{query->object, query->date});
    std::string output_file_path = year_file_path.getPath();

    for (auto& cache_file : cache_meta.getCacheFiles())
    {
        if (cache_file.getFileName() == output_file_path)
        {
            return output_file_path;
        }
    }

    HorizonsPull pull(query->object, query->date.yearRequestStartString(), query->date.yearRequestEndString(), "1d");
    pull.makeRequest(output_file_path + ".raw");

    HorizonOutputFile opf(output_file_path + ".raw");
    HorizonGeneratedEphemeris ephem(opf);
    ParsedHorizonGeneratedEphemeris pephem(ephem);

    writeCacheFile(pephem, output_file_path);

    return output_file_path;
}