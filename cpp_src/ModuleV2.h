// RMT Module V2 Format Prototype
// By VinsCool, 2023
//
// TODO: Move the Legacy Import code to IO_Song.cpp or similar, in order to get most of the CModule functions cleared from unrelated stuff
// TODO: Move most of the Editor Functions to CSong or similar for the same reason
// TODO: Apply to Instrument the same cleanup and organisation done to the majority of Module and Subtune
// TODO: Create new Structures for the Instrument Envelope and Instrument Table data, in order to make things easier to manage later

#pragma once

#include "General.h"
#include "global.h"

// ----------------------------------------------------------------------------
// Data boundaries constant
//
#define INVALID							-1									// Failsafe value for invalid data

// ----------------------------------------------------------------------------
// Module Header definition
//
#define MODULE_VERSION					0									// Module Version number, the highest value is always assumed to be the most recent
#define MODULE_IDENTIFIER				"RMTE"								// Raster Music Tracker Extended, "DUMB" is another potential identifier
#define MODULE_REGION					g_ntsc								// 0 for PAL, 1 for NTSC, anything else is also assumed to be NTSC
#define MODULE_STEREO					g_tracks4_8							// 4 for Mono, 8 for Stereo
#define MODULE_A4_TUNING				g_basetuning						// Default A-4 Tuning
#define MODULE_BASE_NOTE				g_basenote							// Default Base Note (A-)
#define MODULE_TEMPERAMENT				g_temperament						// Default Tuning Temperament
#define MODULE_PRIMARY_HIGHLIGHT		g_trackLinePrimaryHighlight			// Pattern Primary Highlight
#define MODULE_SECONDARY_HIGHLIGHT		g_trackLineSecondaryHighlight		// Pattern Secondary Highlight
#define MODULE_LINE_NUMBERING_MODE		g_tracklinealtnumbering				// Row numbering mode
#define MODULE_LINE_STEP				g_linesafter						// Line Step between cursor movements
#define MODULE_DISPLAY_FLAT_NOTES		g_displayflatnotes					// Display accidentals as Flat instead of Sharp
#define MODULE_DISPLAY_GERMAN_NOTATION	g_usegermannotation					// Display notes using the German Notation
#define MODULE_DEFAULT_SUBTUNE			0									// Default Active Subtune
#define MODULE_DEFAULT_INSTRUMENT		0									// Default Active Instrument
#define MODULE_SUBTUNE_COUNT			1									// Default Subtune Count
#define MODULE_TRACK_LENGTH				64									// Default Track Length
#define MODULE_SONG_LENGTH				1									// Default Song Length
#define MODULE_SONG_SPEED				6									// Default Song Speed
#define MODULE_VBI_SPEED				1									// Default VBI Speed
#define MODULE_TITLE_NAME_MAX			64									// Maximum length of Song Title
#define MODULE_AUTHOR_NAME_MAX			64									// Maximum length of Author name
#define MODULE_COPYRIGHT_INFO_MAX		64									// Maximum length of Copyright info

// ----------------------------------------------------------------------------
// Song and Track Pattern definition
//
#define SUBTUNE_NAME_MAX				64									// 0-63 inclusive, Maximum length of Subtune name
#define SUBTUNE_MAX						64									// 0-63 inclusive, Maximum number of Subtunes in a Module file
#define SONGLINE_MAX					256									// 0-255 inclusive, Songline index used in Song
#define TRACK_CHANNEL_MAX				8									// 0-7 inclusive, 2 POKEY soundchips, each using 4 Channels, a typical Stereo configuration
#define TRACK_ROW_MAX					256									// 0-255 inclusive, Row index used in Pattern
#define TRACK_PATTERN_MAX				256									// 0-255 inclusive, Pattern index used in Song

#define CH1								0									// POKEY Channel identifier for Pattern Column 1
#define CH2								1									// POKEY Channel identifier for Pattern Column 2
#define CH3								2									// POKEY Channel identifier for Pattern Column 3
#define CH4								3									// POKEY Channel identifier for Pattern Column 4
#define CH5								4									// POKEY Channel identifier for Pattern Column 5
#define CH6								5									// POKEY Channel identifier for Pattern Column 6
#define CH7								6									// POKEY Channel identifier for Pattern Column 7
#define CH8								7									// POKEY Channel identifier for Pattern Column 8
#define PATTERN_COLUMN_MAX				8									// 0-7 inclusive, Pattern column index used for Note, Instrument, Volume, and Effect Commands

#define PATTERN_ACTIVE_EFFECT_MAX		4									// 0-3 inclusive, Number of Active Effect columns in Track Channel

#define PATTERN_NOTE_COUNT				120									// 0-119 inclusive, Note index used in Pattern, for a total of 10 octaves
#define PATTERN_NOTE_EMPTY				PATTERN_NOTE_COUNT					// There is no Note in the Pattern Row
#define PATTERN_NOTE_OFF				PATTERN_NOTE_COUNT + 1				// The Note Command OFF will stop the last played note in the Track Channel
#define PATTERN_NOTE_RELEASE			PATTERN_NOTE_COUNT + 2				// The Note Command === will release the last played note in the Track Channel
#define PATTERN_NOTE_RETRIGGER			PATTERN_NOTE_COUNT + 3				// The Note Command ~~~ will retrigger the last played note in the Track Channel
#define PATTERN_NOTE_MAX				PATTERN_NOTE_COUNT + 4				// Total for Note index integrity

#define PATTERN_INSTRUMENT_COUNT		64									// 0-63 inclusive, Instrument index used in Pattern
#define PATTERN_INSTRUMENT_EMPTY		PATTERN_INSTRUMENT_COUNT			// There is no Instrument in the Pattern Row
#define	PATTERN_INSTRUMENT_MAX			PATTERN_INSTRUMENT_COUNT + 1		// Total for Instrument index integrity

#define PATTERN_VOLUME_COUNT			16									// 0-15 inclusive, Volume index used in Pattern
#define PATTERN_VOLUME_EMPTY			PATTERN_VOLUME_COUNT				// There is no Volume in the Pattern Row
#define PATTERN_VOLUME_MAX				PATTERN_VOLUME_COUNT + 1			// Total for Volume index integrity

#define PATTERN_EFFECT_COUNT			16									// 0-15 inclusive, Effect index used in Pattern
#define PATTERN_EFFECT_EMPTY			PATTERN_EFFECT_COUNT				// There is no Effect Command in the Pattern Row
#define PATTERN_EFFECT_MAX				PATTERN_EFFECT_COUNT + 1			// Total for Effect Command index integrity

// ----------------------------------------------------------------------------
// Instrument definition
//
#define INSTRUMENT_NAME_MAX				64									// Maximum length of instrument name
#define INSTRUMENT_PARAMETER_MAX		24									// Instrument parameter 0-23, inclusive
#define INSTRUMENT_TABLE_INDEX_MAX		32									// Instrument note/freq table 0-31, inclusive
#define ENVELOPE_INDEX_MAX				48									// Instrument envelope 0-47, inclusive
#define ENVELOPE_PARAMETER_MAX			8									// Instrument envelope parameter 0-7, inclusive

// ----------------------------------------------------------------------------
// Effect Command definition
//
#define EFFECT_PARAMETER_MAX			0xFF								// 0-255 inclusive, Effect $XY Parameter used in Pattern
#define EFFECT_PARAMETER_MIN			0x00								// The $XY Parameter of 0 may be used to disable certain Effect Commands
#define EFFECT_PARAMETER_DEFAULT		0x80								// The $XY Parameter of 128 may be used to disable certain Effect Commands
//
#define EFFECT_COMMAND_BXX				0x0B								// Effect Command Bxx -> Goto Songline $xx
#define EFFECT_COMMAND_DXX				0x0D								// Effect Command Dxx -> End Pattern, no parameter needed(?)
#define EFFECT_COMMAND_FXX				0x0F								// Effect Command Fxx -> Set Song Speed $xx
//
#define CMD1							0									// Effect Command identifier for Effect Column 1
#define CMD2							1									// Effect Command identifier for Effect Column 2
#define CMD3							2									// Effect Command identifier for Effect Column 3
#define CMD4							3									// Effect Command identifier for Effect Column 4

// ----------------------------------------------------------------------------
// RMTE Module Structs
//

// Effect Command Data, 1 byte for the Identifier, and 1 byte for the Parameter, nothing too complicated
struct TEffect
{
	BYTE identifier;
	BYTE parameter;
};

// Row Data, used within the Pattern data, designed to be easy to manage, following a Row by Row approach
struct TRow
{
	BYTE note;										// Note index, as well as Pattern Commands such as Stop, Release, Retrigger, etc
	BYTE instrument;								// Instrument index
	BYTE volume;									// Volume index
	TEffect command[PATTERN_ACTIVE_EFFECT_MAX];		// Effect Command, toggled from the Active Effect Columns in Track Channels
};

// Pattern Data, indexed by the TRow Struct
struct TPattern
{
	TRow row[TRACK_ROW_MAX];						// Row data is contained withn its associated Pattern index
};

// Channel Index, used for indexing the Songline and Pattern data, similar to the CSong Class
struct TIndex
{
	BYTE effectCommandCount;						// Number of Effect Commands enabled per Track Channel
	BYTE songline[SONGLINE_MAX];					// Pattern Index for each songline within the Track Channel
	TPattern pattern[TRACK_PATTERN_MAX];			// Pattern Data for the Track Channel
};

// Subtune Index, used for indexing all of the Module data, indexed by the TIndex Struct
struct TSubtune
{
	char name[SUBTUNE_NAME_MAX + 1];				// Subtune name
	BYTE songLength;								// Song Length, in Songlines
	BYTE patternLength;								// Pattern Length, in Rows
	BYTE channelCount;								// Number of Channels used in Subtune
	BYTE songSpeed;									// Song Speed, in Frames per Row
	BYTE instrumentSpeed;							// Instrument Speed, in Frames per VBI
	TIndex channel[TRACK_CHANNEL_MAX];				// Channel Index assigned to the Subtune
};

// Instrument Data, due to the Legacy TInstrument struct, this is temporarily defined as TInstrumentV2
struct TInstrumentV2
{
	char name[INSTRUMENT_NAME_MAX + 1];				// Instrument name
	BYTE envelopeLength;							// Envelope Length, in frames
	BYTE envelopeLoop;								// Envelope Loop point, in frames
	BYTE envelopeRelease;							// Envelope Release point, in frames
	BYTE envelopeSpeed;								// Envelope Speed, in frames
	BYTE tableLength;								// Table Length, in frames
	BYTE tableLoop;									// Table Loop point, in frames
	BYTE tableRelease;								// Table Release point, in frames
	BYTE tableMode;									// Table Mode, Absolute or Relative, Additive or Set, any combination thereof
	BYTE tableSpeed;								// Table Speed, in frames
	BYTE volumeEnvelope[ENVELOPE_INDEX_MAX];		// Volume Envelope
	BYTE distortionEnvelope[ENVELOPE_INDEX_MAX];	// Distortion Envelope
	BYTE audctlEnvelope[ENVELOPE_INDEX_MAX];		// AUDCTL Envelope
	BYTE commandEnvelope[ENVELOPE_INDEX_MAX];		// Extended RMT Instrument Commands
	WORD parameterEnvelope[ENVELOPE_INDEX_MAX];		// Extended RMT Instrument Parameters
	BYTE noteTable[INSTRUMENT_TABLE_INDEX_MAX];		// Note Table
	BYTE freqTable[INSTRUMENT_TABLE_INDEX_MAX];		// Freq Table
};

// ----------------------------------------------------------------------------
// RMTE Module Class
//
class CModule
{
public:
	CModule();
	~CModule();
	
	//-- Module Initialisation Functions --//

	void InitialiseModule();
	void ClearModule();

	void CreateSubtune(int subtune);
	void DeleteSubtune(int subtune);
	void InitialiseSubtune(TSubtune* p);

	void CreateInstrument(int instrument);
	void DeleteInstrument(int instrument);
	void InitialiseInstrument(TInstrumentV2* p);

	//-- Legacy RMT Module Import Functions --//

	bool ImportLegacyRMT(std::ifstream& in);
	bool DecodeLegacyRMT(std::ifstream& in, TSubtune* subtune, CString& log);
	bool ImportLegacyPatterns(TSubtune* subtune, BYTE* sourceMemory, WORD sourceAddress);
	bool ImportLegacySonglines(TSubtune* subtune, BYTE* sourceMemory, WORD sourceAddress, WORD endAddress);
	bool ImportLegacyInstruments(TSubtune* subtune, BYTE* sourceMemory, WORD sourceAddress, BYTE version, BYTE* isLoaded);

	//-- Booleans for Module Index and Data integrity --//

	bool IsValidSubtune(int subtune) { return subtune >= 0 && subtune < SUBTUNE_MAX; };
	bool IsValidChannel(int channel) { return channel >= 0 && channel < TRACK_CHANNEL_MAX; };
	bool IsValidSongline(int songline) { return songline >= 0 && songline < SONGLINE_MAX; };
	bool IsValidPattern(int pattern) { return pattern >= 0 && pattern < TRACK_PATTERN_MAX; };
	bool IsValidRow(int row) { return row >= 0 && row < TRACK_ROW_MAX; };
	bool IsValidNote(int note) { return note >= 0 && note < PATTERN_NOTE_MAX; };
	bool IsValidInstrument(int instrument) { return instrument >= 0 && instrument < PATTERN_INSTRUMENT_MAX; };
	bool IsValidVolume(int volume) { return volume >= 0 && volume < PATTERN_VOLUME_MAX; };
	bool IsValidCommand(int command) { return command >= 0 && command < PATTERN_EFFECT_MAX; };
	bool IsValidCommandColumn(int column) { return column >= 0 && column < PATTERN_ACTIVE_EFFECT_MAX; };

	//-- Pointers to Module Data --//

	TSubtune* GetSubtune(BYTE subtune) { return IsValidSubtune(subtune) ? m_subtuneIndex[subtune] : NULL; };
	TIndex* GetChannelIndex(BYTE subtune, BYTE channel);
	TPattern* GetPattern(BYTE subtune, BYTE channel, BYTE pattern);
	TPattern* GetIndexedPattern(BYTE subtune, BYTE channel, BYTE songline);
	TRow* GetRow(BYTE subtune, BYTE channel, BYTE pattern, BYTE row);
	
	//-- Getters and Setters for Pattern and Songline Data --//

	const BYTE GetPatternInSongline(BYTE subtune, BYTE channel, BYTE songline);
	const BYTE GetPatternRowNote(BYTE subtune, BYTE channel, BYTE pattern, BYTE row);
	const BYTE GetPatternRowInstrument(BYTE subtune, BYTE channel, BYTE pattern, BYTE row);
	const BYTE GetPatternRowVolume(BYTE subtune, BYTE channel, BYTE pattern, BYTE row);
	const BYTE GetPatternRowEffectCommand(BYTE subtune, BYTE channel, BYTE pattern, BYTE row, BYTE column);
	const BYTE GetPatternRowEffectParameter(BYTE subtune, BYTE channel, BYTE pattern, BYTE row, BYTE column);

	void SetPatternInSongline(BYTE subtune, BYTE channel, BYTE songline, BYTE pattern);
	void SetPatternRowNote(BYTE subtune, BYTE channel, BYTE pattern, BYTE row, BYTE note);
	void SetPatternRowInstrument(BYTE subtune, BYTE channel, BYTE pattern, BYTE row, BYTE instrument);
	void SetPatternRowVolume(BYTE subtune, BYTE channel, BYTE pattern, BYTE row, BYTE volume);
	void SetPatternRowCommand(BYTE subtune, BYTE channel, BYTE pattern, BYTE row, BYTE column, BYTE effectCommand, BYTE effectParameter);

	//-- Getters and Setters for Module Parameters --//

	const char* GetSongName() { return m_songName; };
	const char* GetSongAuthor() { return m_songAuthor; };
	const char* GetSongCopyright() { return m_songCopyright; };

	const char* GetSubtuneName(BYTE subtune);
	const BYTE GetSongLength(BYTE subtune);
	const BYTE GetPatternLength(BYTE subtune);
	const BYTE GetChannelCount(BYTE subtune);
	const BYTE GetSongSpeed(BYTE subtune);
	const BYTE GetInstrumentSpeed(BYTE subtune);
	const BYTE GetEffectCommandCount(BYTE subtune, BYTE channel);

	void SetSongName(const char* name) { strncpy_s(m_songName, name, MODULE_TITLE_NAME_MAX); };
	void SetSongAuthor(const char* author) { strncpy_s(m_songAuthor, author, MODULE_AUTHOR_NAME_MAX); };
	void SetSongCopyright(const char* copyright) { strncpy_s(m_songCopyright, copyright, MODULE_COPYRIGHT_INFO_MAX); };

	void SetSubtuneName(BYTE subtune, const char* name);
	void SetSongLength(BYTE subtune, BYTE length);
	void SetPatternLength(BYTE subtune, BYTE length);
	void SetChannelCount(BYTE subtune, BYTE count);
	void SetSongSpeed(BYTE subtune, BYTE speed);
	void SetInstrumentSpeed(BYTE subtune, BYTE speed);
	void SetEffectCommandCount(BYTE subtune, BYTE channel, BYTE column);

	//-- RMTE Editor Functions --//

	const BYTE GetSubtuneCount();
	BYTE GetShortestPatternLength(int subtune, int songline);
	BYTE GetShortestPatternLength(TSubtune* subtune, int songline);
	bool DuplicatePatternInSongline(int subtune, int channel, int songline, int pattern);
	bool IsUnusedPattern(int subtune, int channel, int pattern);
	bool IsUnusedPattern(TIndex* index, int pattern);
	bool IsEmptyPattern(int subtune, int channel, int pattern);
	bool IsEmptyPattern(TPattern* pattern);
	bool IsIdenticalPattern(TPattern* sourcePattern, TPattern* destinationPattern);
	bool CopyPattern(TPattern* sourcePattern, TPattern* destinationPattern);
	bool ClearPattern(int subtune, int channel, int pattern);
	bool ClearPattern(TPattern* destinationPattern);
	bool CopyIndex(TIndex* sourceIndex, TIndex* destinationIndex);
	bool CopySubtune(TSubtune* sourceSubtune, TSubtune* destinationSubtune);
	bool DuplicateChannelIndex(int subtune, int sourceIndex, int destinationIndex);
	void MergeDuplicatedPatterns(int subtune);
	void MergeDuplicatedPatterns(TSubtune* subtune);
	void RenumberIndexedPatterns(int subtune);
	void RenumberIndexedPatterns(TSubtune* subtune);
	void ClearUnusedPatterns(int subtune);
	void ClearUnusedPatterns(TSubtune* subtune);
	void ConcatenateIndexedPatterns(int subtune);
	void ConcatenateIndexedPatterns(TSubtune* subtune);
	void AllSizeOptimisations(int subtune);
	void AllSizeOptimisations(TSubtune* subtune);

	//-- Getters and Setters for Instrument Data --//

	TInstrumentV2* GetInstrument(int instrument) { return IsValidInstrument(instrument) ? m_instrumentIndex[instrument] : NULL; };

/*
	const char* GetInstrumentName(int instrument) { return IsValidInstrument(instrument) ? m_instrumentIndex[instrument]->name : "INVALID INSTRUMENT"; };
	const BYTE GetInstrumentEnvelopeLength(int instrument) { return IsValidInstrument(instrument) ? m_instrumentIndex[instrument]->envelopeLength : INVALID; };
	const BYTE GetInstrumentEnvelopeLoop(int instrument) { return IsValidInstrument(instrument) ? m_instrumentIndex[instrument]->envelopeLoop : INVALID; };
	const BYTE GetInstrumentEnvelopeRelease(int instrument) { return IsValidInstrument(instrument) ? m_instrumentIndex[instrument]->envelopeRelease : INVALID; };
	const BYTE GetInstrumentEnvelopeSpeed(int instrument) { return IsValidInstrument(instrument) ? m_instrumentIndex[instrument]->envelopeSpeed : INVALID; };
	const BYTE GetInstrumentTableLength(int instrument) { return IsValidInstrument(instrument) ? m_instrumentIndex[instrument]->tableLength : INVALID; };
	const BYTE GetInstrumentTableLoop(int instrument) { return IsValidInstrument(instrument) ? m_instrumentIndex[instrument]->tableLoop : INVALID; };
	const BYTE GetInstrumentTableRelease(int instrument) { return IsValidInstrument(instrument) ? m_instrumentIndex[instrument]->tableRelease : INVALID; };
	const BYTE GetInstrumentTableMode(int instrument) { return IsValidInstrument(instrument) ? m_instrumentIndex[instrument]->tableMode : INVALID; };
	const BYTE GetInstrumentTableSpeed(int instrument) { return IsValidInstrument(instrument) ? m_instrumentIndex[instrument]->tableSpeed : INVALID; };
	const BYTE* GetInstrumentVolumeEnvelope(int instrument) { return IsValidInstrument(instrument) ? m_instrumentIndex[instrument]->volumeEnvelope : NULL; };
	const BYTE* GetInstrumentDistortionEnvelope(int instrument) { return IsValidInstrument(instrument) ? m_instrumentIndex[instrument]->distortionEnvelope : NULL; };
	const BYTE* GetInstrumentAudctlEnvelope(int instrument) { return IsValidInstrument(instrument) ? m_instrumentIndex[instrument]->audctlEnvelope : NULL; };
	const BYTE* GetInstrumentCommandEnvelope(int instrument) { return IsValidInstrument(instrument) ? m_instrumentIndex[instrument]->commandEnvelope : NULL; };
	const WORD* GetInstrumentParameterEnvelope(int instrument) { return IsValidInstrument(instrument) ? m_instrumentIndex[instrument]->parameterEnvelope : NULL; };
	const BYTE* GetInstrumentNoteTable(int instrument) { return IsValidInstrument(instrument) ? m_instrumentIndex[instrument]->noteTable : NULL; };
	const BYTE* GetInstrumentFreqTable(int instrument) { return IsValidInstrument(instrument) ? m_instrumentIndex[instrument]->freqTable : NULL; };
*/

	void SetInstrumentName(int instrument, const char* name) { if (IsValidInstrument(instrument)) strncpy_s(m_instrumentIndex[instrument]->name, name, INSTRUMENT_NAME_MAX); };


private:
	char m_songName[MODULE_TITLE_NAME_MAX + 1];
	char m_songAuthor[MODULE_AUTHOR_NAME_MAX + 1];
	char m_songCopyright[MODULE_COPYRIGHT_INFO_MAX + 1];
	TSubtune* m_subtuneIndex[SUBTUNE_MAX];
	TInstrumentV2* m_instrumentIndex[PATTERN_INSTRUMENT_COUNT];
};
