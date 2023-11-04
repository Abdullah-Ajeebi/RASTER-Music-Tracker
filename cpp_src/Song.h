#pragma once
#include "stdafx.h"
#include <fstream>

#include "General.h"
#include "global.h"

#include "Undo.h"
#include "Instruments.h"
#include "Tracks.h"
#include "ModuleV2.h"

struct TBookmark
{
	int songline;
	int trackline;
	int speed;
};

struct TSong	//due to Undo
{
	int song[SONGLEN][SONGTRACKS];
	int songgo[SONGLEN];					//if> = 0, then GO applies
	TBookmark bookmark;
};

struct TInfo
{
	char songname[SONG_NAME_MAX_LEN + 1];
	int speed;
	int mainspeed;
	int instrspeed;
	int songnamecur; //to return the cursor to the appropriate position when undo changes in the song name
};

struct TExportMetadata
{
	char songname[SONG_NAME_MAX_LEN + 1];
	CTime currentTime;
	int instrspeed;
	bool isStereo;
	bool isNTSC;
	bool autoRegion;
	bool displayRasterbar;
	int rasterbarColour;
	char atariText[5 * 40];
};

typedef struct
{
	unsigned char mem[65536];				// default RAM size for most 800xl/xe machines

	int targetAddrOfModule;					// Start of RMT module in memory [$4000]
	int firstByteAfterModule;				// Hmm, 1st byte after the RMT module

	BYTE instrumentSavedFlags[INSTRSNUM];
	BYTE trackSavedFlags[TRACKSNUM];

} tExportDescription;


// POKEY registers typically make use of 8-bit values, but there are cases where 2 Channels may also be joined together in 16-bit mode
// It is effectively possible to use the same data for either 2 contiguous AUDF Bytes, or a single 16-bit Word, depending on the AUDCTL state
struct TPokeyRegisters
{
	union
	{
		BYTE audf[4];
		WORD audf16[2];
	};
	BYTE audc[4];
	BYTE audctl;
	BYTE skctl;
};

// POKEY registers buffer Frame, used for every call of Instrument and/or Effect routines
struct TPokeyFrame
{
	TPokeyRegisters pokey[POKEY_SOUNDCHIP_COUNT];
};

// POKEY registers buffer Heap, used for rendering sound for a set number of Millisecond Chunks
struct TPokeyBuffer
{
	TPokeyFrame frame[256];
};

/*
// Active Instrument Envelope Variables
struct TActive
{
	bool isActive;					// Instrument Envelope is Active
	BYTE offset;					// Instrument Envelope Offset
	BYTE timer;						// Instrument Envelope Timer
};

// Periodic Effect Variables, such as Vibrato and Tremolo
struct TPeriodic
{
	bool isActive;
	BYTE depth;
	BYTE speed;
	BYTE phase;
};

// Portamento Effect Variables
struct TPortamento
{
	bool isActive;
	BYTE depth;
	BYTE speed;
	double lastPitch;
	double targetPitch;
};

struct TEnvelopeVariables
{
	TActive volume;
	TActive timbre;
	TActive audctl;
	TActive trigger;
	TActive effect;
	TActive note;
	TActive freq;
};

// Instrument variables used by the RMTE Module playback routines
struct TInstrumentVariables
{
	bool isEnvelopeLooped;			// Instrument Envelope flag for looping, used to trigger Volume Slide if enabled
	TEnvelopeVariables envelope;	// Instrument Envelope variables to keep track of during playback
	BYTE volume;					// AUDC, bits 0-3, Volume level could be 0-15 inclusive
	BYTE timbre;					// AUDC, bits 5-7, unique tones and noises generated by the POKEY Soundchip
	BYTE audctl;					// AUDCTL, bits from all 4 POKEY Channels combined during playback
	TAutomatic trigger;				// Trigger bits used for specific functionalities during playback
	TEffect effect;					// Instrument Effect Command used for specific functionalities during playback
	BYTE note;						// Instrument Note, which may be used as an offset to the Channel Note
	BYTE freq;						// Instrument Freq, which may be used as an offset to the Channel Freq
	BYTE volumeSlide;				// Volume Slide parameter, set using the Axx Command, the Instrument Volume Slide Parameter will take priority if enabled
	BYTE delayTimer;				// Delay Timer used by the ShiftFreq and Vibrato Commands in the Legacy RMT Instrument format, for compatibility's sake
	BYTE finetuneOffset;			// Finetune offset, set using the ?xx Command, signed values are expected, thus $01 will add 1, and $FF will subtract 1, etc
	TPortamento portamento;			// Instrument Portamento, taking priority over Channel Portamento
	TPeriodic vibrato;				// Instrument Vibrato, taking priority over Channel Vibrato
};

// Channel variables used by the RMTE Module playback routines
// Loosely inspired by FamiTracker for most of them
struct TChannelVariables
{
	bool isNoteTrigger;				// Note Trigger, for new Note initialisation, the Command ~~~ will retrigger the last Note played
	bool isNoteRelease;				// Note Release, the Command === will release the last Note played
	bool isNoteActive;				// Note Playing, the Command OFF will stop the last Note played, and reset the Channel variables
	bool isNoteReset;				// Note Reset, implying it is played for the first time when this flag is set
	bool isVolumeOnlyEnabled;		// AUDC, bit 4, forcing Volume Only output in the Channel when enabled
	BYTE note;						// Maximum set by NOTE_COUNT, currently set to 120
	BYTE instrument;				// Maximum set by INSTRUMENT_COUNT, currently set to 64
	BYTE volume;					// AUDC, bits 0-3, Volume level could be 0-15 inclusive
	BYTE timbre;					// AUDC, bits 5-7, unique tones and noises generated by the POKEY Soundchip
	BYTE audctl;					// AUDCTL, bits from all 4 POKEY Channels combined during playback
	BYTE frameCount;				// Counter used for timing Effect Commands, including the Arpeggio Cycle, and possibly more things
	BYTE delayOffset;				// Number of frames used to delay a Row during playback
	TRow* delayedRow;				// Pointer to the delayed Row Index
	BYTE arpeggioScheme;			// Arpeggio Scheme, set using the 0xy Command, cycling between channelNote, x, y, then repeat
	TPortamento portamento;			// Channel Portamento, set using Effect Commands such as 3xy
	TPeriodic vibrato;				// Channel Vibrato, set using Effect Commands such as 4xy
	TPeriodic tremolo;				// Channel Tremolo, set using Effect Commands such as 7xy
	BYTE finetuneOffset;			// Finetune offset, set using the ?xx Command, signed values are expected, thus $01 will add 1, and $FF will subtract 1, etc
};

struct TSongVariables
{
	TChannelVariables channel[CHANNEL_COUNT];
	TInstrumentVariables instrument[CHANNEL_COUNT];
};
*/


// Pattern Editor Cursor Column Index
typedef enum patternCursorColumn_t : BYTE
{
	CC_NOTE = 0, //CC_OCTAVE,
	CC_INSTRUMENT_X, CC_INSTRUMENT_Y,
	CC_VOLUME,
	CC_CMD1_IDENTIFIER, CC_CMD1_X, CC_CMD1_Y,
	CC_CMD2_IDENTIFIER, CC_CMD2_X, CC_CMD2_Y,
	CC_CMD3_IDENTIFIER, CC_CMD3_X, CC_CMD3_Y,
	CC_CMD4_IDENTIFIER, CC_CMD4_X, CC_CMD4_Y,
	CC_INDEX_MAX,
} TPatternCursorColumn;

// Simplified Column Index macros, useful when the exact Cursor position isn't absolutely necessary
#define CC_NOTE(x)			(x == CC_NOTE /*|| x == CC_OCTAVE*/)
#define CC_INSTRUMENT(x)	(x == CC_INSTRUMENT_X || x == CC_INSTRUMENT_Y)
#define CC_VOLUME(x)		(x == CC_VOLUME)
#define CC_CMD1(x)			(x == CC_CMD1_IDENTIFIER || x == CC_CMD1_X || x == CC_CMD1_Y)
#define CC_CMD2(x)			(x == CC_CMD2_IDENTIFIER || x == CC_CMD2_X || x == CC_CMD2_Y)
#define CC_CMD3(x)			(x == CC_CMD3_IDENTIFIER || x == CC_CMD3_X || x == CC_CMD3_Y)
#define CC_CMD4(x)			(x == CC_CMD4_IDENTIFIER || x == CC_CMD4_X || x == CC_CMD4_Y)
#define VALID_CC_INDEX(x)	(x < CC_INDEX_MAX)

// The Effect Command Column Index formula is derived from the Cursor Column Index value itself
#define CC_CMD_INDEX(x)		((x - CC_CMD1_IDENTIFIER) / (CC_CMD2_IDENTIFIER - CC_CMD1_IDENTIFIER))


extern CModule g_Module;

class CSong
{
public:
	CSong();
	~CSong();

	void ChangeTimer(int ms);
	void KillTimer();

	void ClearSong(int numoftracks);

	//void ClearChannelVariables();
	//void CreateChannelVariables(int channel);
	//void DeleteChannelVariables(int channel);
	//void InitialiseChannelVariables();

/*
	void ResetChannelVariables(TChannelVariables* pVariables);

	void ClearSongVariables();
	void CreateSongVariables();
	void DeleteSongVariables();
*/

	void ClearPokeyBuffer();
	void CreatePokeyBuffer();
	void DeletePokeyBuffer();


	//void MidiEvent(DWORD dwParam);

	// Legacy Draw functions
	//void DrawSong();				// Draw the song line info on the right
	//void DrawTracks();
	//void DrawInstrument();
	//void DrawInfo();			//top left corner
	//void DrawAnalyzer();
	//void DrawPlayTimeCounter();

	// RMTE Draw functions
	void DrawSonglines(/*TSubtune* p*/);
	void DrawSubtuneInfos(/*TSubtune* p*/);
	void DrawRegistersState(/*TSubtune* p*/);
	void DrawPatternEditor(/*TSubtune* p*/);
	void DrawInstrumentEditor(/*TSubtune* p*/);
	void DrawDebugInfos(/*TSubtune* p*/);

	//BOOL InfoKey(int vk, int shift, int control);
	//BOOL InfoCursorGotoSongname(int x);
	//BOOL InfoCursorGotoSpeed(int x);
	//BOOL InfoCursorGotoHighlight(int x);
	//BOOL InfoCursorGotoOctaveSelect(int x, int y);
	//BOOL InfoCursorGotoVolumeSelect(int x, int y);
	//BOOL InfoCursorGotoInstrumentSelect(int x, int y);

	//BOOL InstrKey(int vk, int shift, int control);
	void ActiveInstrSet(int instr);
	//void ActiveInstrPrev() { g_Undo.Separator(); int instr = (m_activeInstrument - 1) & 0x3f; ActiveInstrSet(instr); };
	//void ActiveInstrNext() { g_Undo.Separator(); int instr = (m_activeInstrument + 1) & 0x3f; ActiveInstrSet(instr); };

	//const BYTE GetActiveSubtune() { if (!g_Module.IsValidSubtune(m_activeSubtune)) m_activeSubtune = MODULE_DEFAULT_SUBTUNE; return m_activeSubtune; };

	int GetActiveInstr() { return m_activeInstrument; };
	int GetActiveChannel() { return m_activeChannel; };
	int GetActiveLine() { return m_activeRow; };
	int GetPlayLine() { return m_playRow; };
	int GetActiveCursor() { return m_activeCursor; };
	//int GetActiveColumn() { return m_activeColumn; };
	int GetActiveOctave() { return m_activeOctave; };
	int GetActiveVolume() { return m_activeVolume; };

	//void SetActiveSubtune(int subtune) { if (g_Module.IsValidSubtune(subtune)) m_activeSubtune = subtune; };

	//void SetActiveLine(int line) { m_activeRow = line; };
	//void SetPlayLine(int line) { m_playRow = line; };

	//BOOL CursorToSpeedColumn();
	//BOOL ProveKey(int vk, int shift, int control);
	//BOOL TrackKey(int vk, int shift, int control);
	//BOOL TrackCursorGoto(CPoint point);

	// Legacy Pattern Movement functions
	//BOOL TrackUp(int lines);
	//BOOL TrackDown(int lines, BOOL stoponlastline = 1);
	//BOOL TrackLeft(BOOL column = 0);
	//BOOL TrackRight(BOOL column = 0);

	// RMTE Pattern Movement functions
	void PatternUpDownMovement(int rows);
	void PatternLeftRightMovement(int columns);

	// RMTE Channel Movement functions
	void ChannelLeftRightMovement(int channels);

	// RMTE Songline Movement functions
	void SonglineUpDownMovement(int songlines);
	void SonglineLeftRightMovement(int columns);

	// RMTE Subtune Movement functions
	void SeekNextSubtune();
	void SeekPreviousSubtune();

	//BOOL TrackDelNoteInstrVolSpeed(int noteinstrvolspeed) { return g_Tracks.DelNoteInstrVolSpeed(noteinstrvolspeed, SongGetActiveTrack(), m_activeRow); };
	BOOL TrackSetNoteActualInstrVol(int note) { return g_Tracks.SetNoteInstrVol(note, m_activeInstrument, m_activeVolume, SongGetActiveTrack(), m_activeRow); };
	BOOL TrackSetNoteInstrVol(int note, int instr, int vol) { return g_Tracks.SetNoteInstrVol(note, instr, vol, SongGetActiveTrack(), m_activeRow); };
	//BOOL TrackSetInstr(int instr) { return g_Tracks.SetInstr(instr, SongGetActiveTrack(), m_activeRow); };
	BOOL TrackSetVol(int vol) { return g_Tracks.SetVol(vol, SongGetActiveTrack(), m_activeRow); };
	//BOOL TrackSetSpeed(int speed) { return g_Tracks.SetSpeed(speed, SongGetActiveTrack(), m_activeRow); };
	//int TrackGetNote() { return g_Tracks.GetNote(SongGetActiveTrack(), m_activeRow); };
	//int TrackGetInstr() { return g_Tracks.GetInstr(SongGetActiveTrack(), m_activeRow); };
	int TrackGetVol() { return g_Tracks.GetVol(SongGetActiveTrack(), m_activeRow); };
	//int TrackGetSpeed() { return g_Tracks.GetSpeed(SongGetActiveTrack(), m_activeRow); };
	//BOOL TrackSetEnd() { return g_Tracks.SetEnd(SongGetActiveTrack(), m_activeRow + 1); };
	//int TrackGetLastLine() { return g_Tracks.GetLastLine(SongGetActiveTrack()); };
	//BOOL TrackSetGo() { return g_Tracks.SetGo(SongGetActiveTrack(), m_activeRow); };
	//int TrackGetGoLine() { return g_Tracks.GetGoLine(SongGetActiveTrack()); };
	//void RespectBoundaries();
	//void TrackGetLoopingNoteInstrVol(int track, int& note, int& instr, int& vol);

	int* GetUECursor(int part);
	void SetUECursor(int part, int* cursor);
	BOOL UECursorIsEqual(int* cursor1, int* cursor2, int part);
	BOOL Undo() { return g_Undo.Undo(); };
	int	 UndoGetUndoSteps() { return g_Undo.GetUndoSteps(); };
	BOOL Redo() { return g_Undo.Redo(); };
	int  UndoGetRedoSteps() { return g_Undo.GetRedoSteps(); };

	//BOOL SongKey(int vk, int shift, int control);
	//BOOL SongCursorGoto(CPoint point);

	// Legacy Songline Movement functions
	//BOOL SongUp();
	//BOOL SongDown();
	//BOOL SongSubsongPrev();
	//BOOL SongSubsongNext();

	//BOOL SongTrackSet(int t);
	//BOOL SongTrackSetByNum(int num);
	//BOOL SongTrackDec();
	//BOOL SongTrackInc();
	//BOOL SongTrackEmpty();
	int SongGetActiveTrack() { return (m_songgo[m_activeSongline] >= 0) ? -1 : m_song[m_activeSongline][m_activeChannel]; };
	int SongGetTrack(int songline, int trackcol) { return m_song[songline][trackcol]; /*IsValidSongline(songline) && !IsSongGo(songline) ? m_song[songline][trackcol] : -1;*/ };
	//int SongGetActiveTrackInColumn(int column) { return m_song[m_activeSongline][column]; };
	int SongGetActiveLine() { return m_activeSongline; };
	//int SongGetPlayLine() { return m_playSongline; };
	void SongSetActiveLine(int line) { m_activeSongline = line; };
	//void SongSetPlayLine(int line) { m_playSongline = line; };

	//BOOL SongTrackGoOnOff();
	//int SongGetGo() { return m_songgo[m_activeSongline]; };
	int SongGetGo(int songline) { return m_songgo[songline]; };
	//void SongTrackGoDec() { m_songgo[m_activeSongline] = (m_songgo[m_activeSongline] - 1) & 0xff; };
	//void SongTrackGoInc() { m_songgo[m_activeSongline] = (m_songgo[m_activeSongline] + 1) & 0xff; };

	BOOL SongInsertLine(int line);
	BOOL SongDeleteLine(int line);
	BOOL SongInsertCopyOrCloneOfSongLines(int& line);
	BOOL SongPrepareNewLine(int& line, int sourceline = -1, BOOL alsoemptycolumns = 1);
	int FindNearTrackBySongLineAndColumn(int songline, int column, BYTE* arrayTRACKSNUM);
	BOOL SongPutnewemptyunusedtrack();
	BOOL SongMaketracksduplicate();

	//BOOL OctaveUp() { if (m_activeOctave < 4) { m_activeOctave++; return 1; } else return 0; };
	//BOOL OctaveDown() { if (m_activeOctave > 0) { m_activeOctave--; return 1; } else return 0; };
	//BOOL VolumeUp() { if (m_activeVolume < MAXVOLUME) { m_activeVolume++; return 1; } else return 0; };
	//BOOL VolumeDown() { if (m_activeVolume > 0) { m_activeVolume--; return 1; } else return 0; };

	//void ClearBookmark() { m_bookmark.songline = m_bookmark.trackline = m_bookmark.speed = -1; };
	//BOOL IsBookmark() { return (m_bookmark.speed > 0 && m_bookmark.trackline < g_Tracks.GetMaxTrackLength()); };
	//BOOL SetBookmark();

	void OctaveUp() { if (++m_activeOctave > 9) m_activeOctave = 9; };
	void OctaveDown() { if (--m_activeOctave < 0) m_activeOctave = 0; };
	void VolumeUp() { if (++m_activeVolume > MAXVOLUME) m_activeVolume = MAXVOLUME; };
	void VolumeDown() { if (--m_activeVolume < 0) m_activeVolume = 0; };
	void InstrumentRight() { if (++m_activeInstrument > INSTRUMENT_COUNT - 1) m_activeInstrument = 0; };
	void InstrumentLeft() { if (--m_activeInstrument < 0) m_activeInstrument = INSTRUMENT_COUNT - 1; };

	void Play(int mode, BOOL follow, int special = 0);
	void Stop();

	//BOOL SongPlayNextLine();
	//BOOL PlayBeat();
	//BOOL PlayVBI();
	//BOOL PlayPressedTonesInit();
	//BOOL SetPlayPressedTonesTNIV(int t, int n, int i, int v) { m_playptnote[t] = n; m_playptinstr[t] = i; m_playptvolume[t] = v; return 1; };
	//BOOL SetPlayPressedTonesV(int t, int v) { m_playptvolume[t] = v; return 1; };
	//BOOL SetPlayPressedTonesSilence();
	//BOOL PlayPressedTones();

	void TimerRoutine();

	void UpdatePlayTime() { m_playTimeFrameCount += m_playMode != MPLAY_STOP ? 1 : 0; };
	void ResetPlayTime() { m_playTimeFrameCount = 0; };

	void CalculatePlayTime();
	void CalculatePlayBPM();
	void CalculateDisplayFPS();

	void SetRMTTitle();

	void FileOpen(const char* filename = NULL, BOOL warnOfUnsavedChanges = TRUE);
	void FileReload();
	BOOL FileCanBeReloaded() { return (m_fileName != "") /*&& (!m_fileunsaved)*/ /*&& g_changes*/; };
	bool WarnUnsavedChanges();

	void FileSave();
	void FileSaveAs();
	void FileNew();
	void FileImport();
	void FileExportAs();

	void FileInstrumentSave();
	void FileInstrumentLoad();
	void FileTrackSave();
	void FileTrackLoad();

	void StrToAtariVideo(char* txt, int count);
	int SongToAta(unsigned char* dest, int max, int adr);
	BOOL AtaToSong(unsigned char* sour, int len, int adr);

	bool CreateExportMetadata(int iotype, struct TExportMetadata* metadata);
	bool WriteToXEX(struct TExportMetadata* metadata);

	bool SaveTxt(std::ofstream& ou);
	bool SaveRMW(std::ofstream& ou);

	bool SaveRMTE(std::ofstream& ou);
	bool LoadRMTE(std::ifstream& in);

	//bool MakeModuleHeader(BYTE* pMem, int& addressOfModuleIndex, int& addressOfSubtuneIndex, int& addressOfInstrumentIndex, int& addressOfDataBlock);

	//bool DecodeRMTE(BYTE* pMem, int& addressOfModuleIndex, int& addressOfSubtuneIndex, int& addressOfInstrumentIndex, BYTE& moduleVersion);

	bool LoadRMT(std::ifstream& in);
	bool LoadTxt(std::ifstream& in);
	bool LoadRMW(std::ifstream& in);

	int ImportTMC(std::ifstream& in);
	int ImportMOD(std::ifstream& in);

	bool ExportV2(std::ofstream& ou, int iotype, LPCTSTR filename = NULL);
	bool ExportAsRMT(std::ofstream& ou, tExportDescription* exportDesc);
	bool ExportAsStrippedRMT(std::ofstream& ou, tExportDescription* exportDesc, LPCTSTR filename);
	bool ExportAsAsm(std::ofstream& ou, tExportDescription* exportStrippedDesc);
	bool ExportAsRelocatableAsmForRmtPlayer(std::ofstream& ou, tExportDescription* exportStrippedDesc);

	bool ExportSAP_R(std::ofstream& ou);
	bool ExportLZSS(std::ofstream& ou, LPCTSTR filename);
	bool ExportCompactLZSS(std::ofstream& ou, LPCTSTR filename);
	bool ExportLZSS_SAP(std::ofstream& ou);
	bool ExportLZSS_XEX(std::ofstream& ou);

	bool ExportWav(std::ofstream& ou, LPCTSTR filename);

	void DumpSongToPokeyBuffer(int playmode = MPLAY_START, int songline = 0, int trackline = 0);
	int BruteforceOptimalLZSS(unsigned char* src, int srclen, unsigned char* dst);

	bool TestBeforeFileSave();
	//int GetSubsongParts(CString& resultstr);

	void ComposeRMTFEATstring(CString& dest, const char* filename, BYTE* instrumentSavedFlags, BYTE* trackSavedFlags, BOOL sfx, BOOL gvf, BOOL nos, int assemblerFormat);

	BOOL BuildRelocatableAsm(CString& dest,
		tExportDescription* exportDesc,
		CString strAsmStartLabel,
		CString strTracksLabel,
		CString strSongLinesLabel,
		CString strInstrumentsLabel,
		int assemblerFormat,
		BOOL sfx,
		BOOL gvf,
		BOOL nos,
		bool bWantSizeInfoOnly);

	int BuildInstrumentData(
		CString& strCode,
		CString strInstrumentsLabel,
		unsigned char* buf,
		int from,
		int to,
		int* info,
		int assemblerFormat
	);

	int BuildTracksData(
		CString& strCode,
		CString strTracksLabel,
		unsigned char* buf,
		int from,
		int to,
		int* track_pos,
		int assemblerFormat);

	int BuildSongData(
		CString& strCode,
		CString strSongLinesLabel,
		unsigned char* buf,
		int offsetSong,
		int len,
		int start,
		int numTracks,
		int assemblerFormat
	);

	void MarkTF_USED(BYTE* arrayTRACKSNUM);
	void MarkTF_NOEMPTY(BYTE* arrayTRACKSNUM);

	//int MakeTuningBlock(unsigned char* mem, int addr);
	//int DecodeTuningBlock(unsigned char* mem, int fromAddr, int endAddr);
	void ResetTuningVariables();

	int MakeModule(unsigned char* mem, int adr, int iotype, BYTE* instrumentSavedFlags, BYTE* trackSavedFlags);
	int MakeRMFModule(unsigned char* mem, int adr, BYTE* instrumentSavedFlags, BYTE* trackSavedFlags);
	int DecodeModule(unsigned char* mem, int adrfrom, int adrend, BYTE* instrumentLoadedFlags, BYTE* trackLoadedFlags);

	void TrackCopy();
	void TrackPaste();
	void TrackCut();
	void TrackDelete();
	void TrackCopyFromTo(int fromtrack, int totrack);
	void TrackSwapFromTo(int fromtrack, int totrack);

	void BlockPaste(int special = 0);

	void InstrCopy();
	void InstrPaste(int special = 0);
	void InstrCut();
	void InstrDelete();

	void InstrInfo(int instr, TInstrInfo* iinfo = NULL, int instrto = -1);
	void InstrChange(int instr);
	void TrackInfo(int track);

	void SongCopyLine();
	void SongPasteLine();
	void SongClearLine();

	void TracksOrderChange();
	void Songswitch4_8(int tracks4_8);
	int GetEffectiveMaxtracklen();
	int GetSmallestMaxtracklen(int songline);
	void ChangeMaxtracklen(int maxtracklen);
	void TracksAllBuildLoops(int& tracksmodified, int& beatsreduced);
	void TracksAllExpandLoops(int& tracksmodified, int& loopsexpanded);
	void SongClearUnusedTracksAndParts(int& clearedtracks, int& truncatedtracks, int& truncatedbeats);

	int SongClearDuplicatedTracks();
	int SongClearUnusedTracks();
	int ClearAllInstrumentsUnusedInAnyTrack();

	void RenumberAllTracks(int type);
	void RenumberAllInstruments(int type);

	//CString GetFilename() { return m_fileName; };
	//int GetFiletype() { return m_fileType; };

	int(*GetSong())[SONGLEN][SONGTRACKS]{ return &m_song; };
	int(*GetSongGo())[SONGLEN] { return &m_songgo; };
	TBookmark* GetBookmark() { return &m_bookmark; };

	int GetPlayMode() { return m_playMode; };
	//void SetPlayMode(int mode) { m_playMode = mode; };

	BOOL GetFollowPlayMode() { return m_isFollowPlay; };
	void SetFollowPlayMode(BOOL follow) { m_isFollowPlay = follow; };

	void GetSongInfoPars(TInfo* info) { memcpy(info->songname, m_songname, SONG_NAME_MAX_LEN); info->speed = m_playSpeed; info->mainspeed = m_mainSpeed; info->instrspeed = m_instrumentSpeed; info->songnamecur = m_songnamecur; };
	void SetSongInfoPars(TInfo* info) { memcpy(m_songname, info->songname, SONG_NAME_MAX_LEN); m_playSpeed = info->speed; m_mainSpeed = info->mainspeed; m_instrumentSpeed = info->instrspeed; m_songnamecur = info->songnamecur; };

	//BOOL IsValidSongline(int songline) { return songline >= 0 && songline < SONGLEN; };
	//BOOL IsSongGo(int songline) { return IsValidSongline(songline) ? m_songgo[songline] >= 0 : 0; };


	// Prototype C++ RMTE Module Driver functions

/*
	void PlayFrame(TSubtune* pSubtune);
	void PlayContinue(TSubtune* pSubtune);
	
	void PlayInstrument(TInstrumentV2* pInstrument, TChannelVariables* pChannelVariables, TInstrumentVariables* pInstrumentVariables);
	bool AdvanceEnvelope(TActive* pActive, TParameter* pParameter, TFlag* pFlag, bool trigger, bool release, bool& hasLooped);
	double GetVibrato(TPeriodic* pVibrato, double pitch);
	double GetPortamento(TPortamento* pPortamento, double pitch);

	void PlayRow(TSubtune* pSubtune);

	void ProcessNote(BYTE note, TChannelVariables* pVariables);
	void ProcessInstrument(BYTE instrument, TChannelVariables* pVariables);
	void ProcessVolume(BYTE volume, TChannelVariables* pVariables);
	void ProcessEffect(TEffect* effect, TChannelVariables* pVariables);

	void PlaybackRespectBoundaries(TSubtune* pSubtune);
*/

	bool TransposeNoteInPattern(int semitone);
	bool TransposePattern(int semitone);
	bool TransposeSongline(int semitone);

	bool SetNoteInPattern(UINT note);
	bool SetInstrumentInPattern(UINT instrument);
	bool SetVolumeInPattern(UINT volume);
	bool SetCommandIdentifierInPattern(UINT command);
	bool SetCommandParameterInPattern(UINT parameter);

	bool SetEmptyRowInPattern();

	bool DeleteRowInPattern();
	bool InsertRowInPattern();

	bool SetPatternInSongline(UINT pattern);
	bool DuplicatePatternInSongline();
	bool SetNewEmptyPatternInSongline();

	void ChangeEffectCommandColumnCount(int offset);

private:
	// Legacy variables
	int m_song[SONGLEN][SONGTRACKS];	// TODO: Delete
	int m_songgo[SONGLEN];				// TODO: Delete	// If >= 0, then GO applies

	BYTE m_activeSubtune;

	BOOL volatile m_isFollowPlay;
	int volatile m_playMode;

	int m_activeSongline;				// Which Songline is currently active
	int volatile m_playSongline;		// Which Songline is currently played

	int m_activeRow;					// Which Row is currently active
	int volatile m_playRow;				// Which Row is currently played

	int m_activeChannel;				// Active POKEY Channel

	int m_activeCursor;					// 0 -> Note, 1 -> Instrument, 2 -> Volume, 3-6 -> Command
	//int m_activeColumn;					// 0-3 -> Cursor Offset for Instrument and Command values

	int m_activeSonglineColumn;			// 0-1 -> Active Songline Nybble Cursor for Editor

	int m_playBlockStart;
	int m_playBlockEnd;

	int m_activeInstrument;
	int m_activeVolume;
	int m_activeOctave;

	int volatile m_mainSpeed;				// TODO: Delete

	BYTE volatile m_playSpeed;
	BYTE volatile m_speedTimer;
	BYTE volatile m_instrumentSpeed;

	int volatile m_nextSongline;
	int volatile m_nextRow;


	//MIDI input variables, used for tests through MIDI CH15 
	//int m_mod_wheel = 0;			// TODO: Delete
	//int m_vol_slider = 0;			// TODO: Delete
	//int m_heldkeys = 0;				// TODO: Delete
	//int m_midi_distortion = 0;		// TODO: Delete
	//BOOL m_ch_offset = 0;			// TODO: Delete

	//POKEY EXPLORER variables, used for tests involving pitch calculations and sound debugging displayed on screen
	//int e_ch_idx = 0;				// TODO: Delete
	//int e_modoffset = 1;			// TODO: Delete
	//int e_coarse_divisor = 1;		// TODO: Delete
	//int e_modulo = 0;				// TODO: Delete
	//BOOL e_valid = 1;				// TODO: Delete
	//double e_divisor = 1;			// TODO: Delete
	//double e_pitch = 0;				// TODO: Delete

	int m_infoact;							// TODO: Delete	// Which part of the info area is active for editing: 0 = name, 
	char m_songname[SONG_NAME_MAX_LEN + 1];	// TODO: Delete
	int m_songnamecur;						// TODO: Delete
	TBookmark m_bookmark;					// TODO: Delete

	// Used for calculating Play time
	int m_playTimeFrameCount;
	int m_playTimeSecondCount;
	int m_playTimeMinuteCount;
	int m_playTimeMillisecondCount;

	// Used for calculating Average BPM
	BYTE m_rowSpeed[8];
	double m_averageBPM;
	double m_averageSpeed;
	
	// Used for calculating Average FPS
	uint64_t m_lastDeltaCount;
	uint64_t m_lastFrameCount;
	uint64_t m_lastMillisecondCount;
	uint64_t m_lastSecondCount;
	double m_averageFrameCount;

	//int volatile m_quantization_note;		// TODO: Delete
	//int volatile m_quantization_instr;		// TODO: Delete
	//int volatile m_quantization_vol;		// TODO: Delete

	//int m_playptnote[SONGTRACKS];			// TODO: Delete
	//int m_playptinstr[SONGTRACKS];			// TODO: Delete
	//int m_playptvolume[SONGTRACKS];			// TODO: Delete

	TInstrument m_instrclipboard;			// TODO: Delete
	int m_songlineclipboard[SONGTRACKS];	// TODO: Delete
	int m_songgoclipboard;					// TODO: Delete

	UINT m_timerRoutine;

	CString m_fileName;
	int m_fileType;
	int m_lastExportType;					// Which data format was used to export a file the last time?

	int m_TracksOrderChange_songlinefrom; // TODO: Delete	//is defined as a member variable to keep in use
	int m_TracksOrderChange_songlineto;	  // TODO: Delete	//the last values used remain

	// RMTE variables
	//TSongVariables* m_songVariables;
	TPokeyBuffer* m_pokeyBuffer;
};