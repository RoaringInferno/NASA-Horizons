#pragma once

#include "horizons/horizons.hpp"
#include "horizons/horizon_output_parsing.hpp"
#include "cache/cache.hpp"
#include "cache/filenames.hpp"
#include "cache/cache_file.hpp"
#include "dates.hpp"

#include <string>

typedef struct {
    std::string object;
    Date date;
} Query;

// Returns the path to the file that was pulled
std::string pullDay(Query* query, const CacheMeta& cache_meta)
{
    DaySaveFilePath day_file_path(DayFileID{query->object, query->date});
    std::string output_file_path = day_file_path.getPath();

    for (auto& cache_file : cache_meta.getCacheFiles())
    {
        if (cache_file.getFileName() == output_file_path)
        {
            return output_file_path;
        }
    }
    
    HorizonsPull pull(query->object, query->date.dayRequestStartString(), query->date.dayRequestEndString(), "1m");
    pull.makeRequest("horizon-day-pulls/" + output_file_path + ".raw");

    HorizonOutputFile opf(output_file_path + ".raw");
    HorizonGeneratedEphemeris ephem(opf);
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

    HorizonOutputFile opf("horizon-year-pulls/" + output_file_path + ".raw");
    HorizonGeneratedEphemeris ephem(opf);
    ParsedHorizonGeneratedEphemeris pephem(ephem);

    writeCacheFile(pephem, output_file_path);

    return output_file_path;
}