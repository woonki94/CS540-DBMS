#include <string>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <bitset>
using namespace std;

const int BLOCK_SIZE = 4096; // initialize the  block size allowed in main memory according to the question


class Record
{
public:
    int id, manager_id;
    std::string bio, name;

    Record(vector<std::string> fields)
    {
        id = stoi(fields[0]);
        name = fields[1];
        bio = fields[2];
        manager_id = stoi(fields[3]);
    }

    void print()
    {
        cout << "\tID: " << id << "\n";
        cout << "\tNAME: " << name << "\n";
        cout << "\tBIO: " << bio << "\n";
        cout << "\tMANAGER_ID: " << manager_id << "\n";
    }
};

struct Slot
{
    int offset; // offset of record in page
    int length; // length of record
};

class Page
{
public:
    char data[BLOCK_SIZE];
    vector<Slot> slots;
    int curOffset;
    int slotDirOffset;

    // constructor
    Page() : curOffset(0), slotDirOffset(BLOCK_SIZE)
    {
        memset(data, 0, BLOCK_SIZE); // Initialize buffer to 0
    }

    // this function adds the current record to page
    bool addRecordToPage(const string &record)
    {

        // cout << "==========INSIDE addRecordToPage()============" << endl;

        int totalSlotSpace = sizeof(Slot) * (slots.size() + 1); // total space needed for the slot directory after adding the new slot.
        int spaceNeeded = record.size() + totalSlotSpace;       // total space needed to store the new record and its corresponding slot in the slot directory.

        // check if there is space for new record and its slot
        if (curOffset + spaceNeeded > BLOCK_SIZE)
        {
            // if not
            return false;
        }

        // copy the record's bytes into the buffer at the current offset.
        memcpy(data + curOffset, record.c_str(), record.size());

        // cout << "slotDirOffset: " << slotDirOffset << endl;

        Slot newSlot = {curOffset, static_cast<int>(record.size())}; // create new slot

        // Debug print
        cout << "Adding record: Offset = " << newSlot.offset << ", Length = " << newSlot.length << endl;

        // UPdate slot directory
        slotDirOffset -= sizeof(Slot);

        memcpy(data + slotDirOffset, &newSlot, sizeof(Slot));
        // Add the slot to slot directory
        slots.push_back(newSlot);
        // update current offset
        curOffset += record.size();

        return true;
    }

    // clear page for reuse
    void clear()
    {
        memset(data, 0, BLOCK_SIZE);
        slots.clear();
        curOffset = 0;
        slotDirOffset = BLOCK_SIZE;
    }

    string serializePage()
    {
        // copy the records of current page into curPageData
        string curPageData(data, curOffset);

        // Debug output: Print the current page data
        //cout << "Page Data Before Slot Directory: " << curPageData << endl;

        cout << "curOffset BEFORE========: " << curOffset << endl;

        int slotNum = slots.size(); // number of slots
        for (const Slot &slot : slots)
        {
            curPageData.append(reinterpret_cast<const char *>(&slot), sizeof(Slot));
            curOffset += sizeof(Slot); // Move past the serialized slot

            // Debug output: Print each slot's details
            cout << "Adding Slot - Offset: " << slot.offset << ", Length: " << slot.length << endl;
        }

        // contentSize = curOffset;

        cout << "curOffset AFTER========: " << curOffset << endl;
        // Now account for the space that will be taken by 'slotNum'
        // curOffset += sizeof(int); // Assuming slotNum is an int
        // cout << "curOffset AFTER========: " << curOffset << endl;

        // cout << "Page Data After Slot Directory : " << curPageData << endl;
        
        //Debug Page Data After appending Slot Directory.
        /*
        cout << "Page Data After Slot Directory : ";
        for (int i = 0; i < BLOCK_SIZE; ++i)
        {
            char ch = curPageData[i];
            if (isprint(ch))
            {
                cout << ch; // Print as is if printable
            }
            else
            {
                cout << "."; // Print a placeholder for non-printable characters
            }
        }
        cout << endl;
        cout << endl;
       
        for (unsigned char c : curPageData)
        {
            cout << hex << setw(2) << setfill('0') << (int)c << " ";
        }
        cout << dec << endl; // Switch back to decimal format
        */ 
        // append slot directoy at the end of curPageData
        // curPageData.append(data + slotDirOffset, BLOCK_SIZE - slotDirOffset);

        cout << "slotNum before appending: " << slotNum << endl;

        // add number of slots to curPageData
        curPageData.append(reinterpret_cast<const char *>(&slotNum), sizeof(slotNum));
        cout << "Appending numSlots to Page: " << slotNum << endl;

        //Debug pageData after appending Slot Number
        /*
        cout << "Page Data After Slot Number    : ";
        for (int i = 0; i < BLOCK_SIZE; ++i)
        {
            char ch = curPageData[i];
            if (isprint(ch))
            {
                cout << ch; // Print as is if printable
            }
            else
            {
                cout << "."; // Print a placeholder for non-printable characters
            }
        }
        cout << endl;
        cout << endl;
        
        for (unsigned char c : curPageData)
        {
            cout << hex << setw(2) << setfill('0') << (int)c << " ";
        }
        cout << dec << endl; // Switch back to decimal format
        */
        // cout << "curPageData        " << curPageData << endl;

        // cout << "slotDirOffset: " << slotDirOffset << endl;

        // print slot directory:
        // string slotDir(data + slotDirOffset, BLOCK_SIZE - slotDirOffset);

        int paddingSize = BLOCK_SIZE - curPageData.size();

        if (paddingSize > 0)
        {
            curPageData.append(paddingSize, '#');
        }

        cout << "Total Slots: " << slotNum << endl;
        cout << "_______curPageData.size()________: " << curPageData.size() << endl;

        return curPageData;
    }

    //=======================================================

    void debugPrintPageStructure()
    {
        cout << "Page Structure:" << endl;
        cout << "Records:" << endl;
        for (const auto &slot : slots)
        {
            cout << "  Offset: " << slot.offset << ", Length: " << slot.length << endl;
        }
        cout << "Slot Directory at Offset: " << slotDirOffset << endl;
        cout << "Total Slots: " << slots.size() << endl;
    }

    // Method to print a record from a page given a slot index
    void printRecord(int slotIndex)
    {
        if (slotIndex < 0 || slotIndex >= slots.size())
        {
            cout << "Invalid slot index." << endl;
            return;
        }

        const Slot &slot = slots[slotIndex];

        // Ensure slot is within page boundaries
        if (slot.offset < 0 || slot.offset + slot.length > BLOCK_SIZE)
        {
            cout << "Invalid slot detected." << endl;
            return;
        }

        // Extract and print record data
        string recordData(data + slot.offset, slot.length);
        cout << "Record at slot " << slotIndex << ": " << recordData << endl;
    }

    // Function to print the content of the slot directory
    void printSlotDirectory() const
    {
        cout << "Slot Directory (Total Slots: " << slots.size() << "):" << endl;
        for (const Slot &slot : slots)
        {
            cout << "Slot Offset: " << slot.offset << ", Length: " << slot.length << endl;
        }
    }

    //=======================================================
};

class StorageBufferManager
{
private:
    // You may declare variables based on your need
    ofstream of;        // file stream for output file
    Page page[3];       // buffer for up to 3 pages in main memory
    int numRecords = 0; // number of inserted records
    int curPageIdx = 0; // index of current page in memory

    string serialize(const Record &record)
    {
        ostringstream serializedRecord;

        serializedRecord.write(reinterpret_cast<const char *>(&record.id), sizeof(record.id));                 // serialize int id field, fixed size
        serializedRecord << record.name << ',';                                                                // serialize string name, variable length
        serializedRecord << record.bio << ',';                                                                 // serialize string bio, variable length
        serializedRecord.write(reinterpret_cast<const char *>(&record.manager_id), sizeof(record.manager_id)); // serialize int manager id field, fixed size

        return serializedRecord.str();
    }

    // Insert new record
    void insertRecord(Record record)
    {
        string serializedRecord = serialize(record);

        // cout << "serializedRecord:    " << serializedRecord << endl;

        // cout << "============INSIDE insertRecord()==============" << endl;

        bool isRecordAdded = page[curPageIdx].addRecordToPage(serializedRecord); // this return true if record is successfully added to page

        page[curPageIdx].printSlotDirectory(); // Print the slot directory for debugging

        if (!isRecordAdded)
        {

            cout << "Space FULL!!" << endl;

            //=======================================================
            // Page myPage; // Assuming this is an existing page with records

            // cout << "=======================================================" << endl;

            cout << "myPage.slots.size(): " << page[curPageIdx].slots.size() << endl;
            page[curPageIdx].debugPrintPageStructure();

            // Loop through all slots and print each record
            // for (int i = 0; i < page[curPageIdx].slots.size(); ++i) {
            //     page[curPageIdx].printRecord(i);
            // }

            // cout << "=======================================================" << endl;

            //=======================================================

            // if there is not enough space for record, page is full
            string pageData = page[curPageIdx].serializePage(); // serialize page

            // cout << "pageData                       : " << pageData << endl;
            // cout << "pageData.size(): " << pageData.size() << endl;

            of.write(pageData.c_str(), pageData.size()); // write page to disk
            of.flush();
            
            //Debug page index.
            //cout << "curPageIdx" <<curPageIdx;
            
            // clear the page for reuse. Ensure page in main memory is under 3.
            page[curPageIdx].clear();
            //Debug address of current place, and slot's place.
            cout << "page[curPageIdx].curOffset: " << page[curPageIdx].curOffset << endl;
            cout << "page[curPageIdx].slotDirOffset: " << page[curPageIdx].slotDirOffset << endl;

            // add current record to page
            bool addedRecord = page[curPageIdx].addRecordToPage(serializedRecord);
            // cout << "============" << endl;
            // cout << "page[curPageIdx].curOffset: " << page[curPageIdx].curOffset << endl;
            // cout << "page[curPageIdx].slotDirOffset: " << page[curPageIdx].slotDirOffset << endl;
        }
        numRecords++;
        // Take neccessary steps if capacity is reached (you've utilized all the blocks in main memory)
    }

    // when all records are read and current page is not full -> not triggered to write to file
    // write it to file
    void addPageTofile()
    {
        // if page has any data, add it to file
        if (page[curPageIdx].curOffset > 0)
        {
            string pageData = page[curPageIdx].serializePage(); // serialize page
            of.write(pageData.c_str(), pageData.size());        // write page to disk
            of.flush();
        }
    }

public:
    StorageBufferManager(string NewFileName)
    {

        // initialize your variables
        //  numRecords = 0;
        //  curPageIdx = 0;

        // Create your EmployeeRelation file
        of.open(NewFileName, ios::binary);
        // check if successful
        if (!of.is_open())
        {
            cerr << "Error opening/creating file: " << NewFileName << endl;
            exit(1);
        }
    }

    // Destructor
    ~StorageBufferManager()
    {
        if (of.is_open())
        {
            of.flush(); // Flush the stream
            of.close(); // Close the file stream
        }
    }

    // Read csv file (Employee.csv) and add records to the (EmployeeRelation)
    void createFromFile(string csvFName)
    {
        ifstream file(csvFName); // open csv file
        if (!file.is_open())
        {
            cerr << "Error opening file: " << csvFName << endl;
            exit(1);
        }

        string parseStr; // hold each line in csv file

        while (getline(file, parseStr))
        {                                      // read in each line
            istringstream strStrean(parseStr); // string stream
            vector<string> fields;             // this holds all the fields of record
            string field;

            while (getline(strStrean, field, ','))
            {                            // read segments (fields) of strStream into field, stop by ','
                fields.push_back(field); // add extracted field into fields vector
            }

            // create a record with extracted fields
            Record record(fields);

            // record.print();
            // insert new record into page
            insertRecord(record);
            // record.print();
        }

        cout << "number of records: " << numRecords << endl;

        // We need to write any remaining data to file
        // After reading all records, check if the current page has unsaved data
        addPageTofile();

        file.close(); // close the file when done
    }

    // Given an ID, find the relevant record and print it
    Record findRecordById(int id)
    {
        ifstream file("EmployeeRelation", ios::binary);
        if (!file.is_open())
        {
            cerr << "Error opening file EmployeeRelation." << endl;
            return Record({"-1", "", "", "-1"}); // Dummy record
        }

        cout << "INSIDE findRecordById(int id)" << endl;

        char pageData[BLOCK_SIZE];
        while (file.read(pageData, BLOCK_SIZE))
        {

            int numSlotsOff = 0;

            for (int i = BLOCK_SIZE - 1; i >= 0; i--)
            {
                if (pageData[i] != '#')
                {
                    // Found the end of actual content, return the position for numSlots
                    numSlotsOff = i + 1 - sizeof(int); // Subtract the size of numSlots
                    break;
                }
            }

            // cout << "numSlotsOff:________: " << numSlotsOff << endl;
            int slotNum = *reinterpret_cast<int *>(pageData + numSlotsOff);
            cout << "numSlots:________: " << slotNum << endl;

            // Print the bytes in hex format
            /*
            cout << "Raw bytes at end of page (for numSlots): ";
            for (int i = numSlotsOff; i < BLOCK_SIZE; ++i)
            {
                unsigned char byte = static_cast<unsigned char>(pageData[i]);
                cout << hex << setw(2) << setfill('0') << static_cast<int>(byte) << " ";
            }
            cout << dec << endl; // Switch back to decimal format
            */
            // int slotNum;
            // memcpy(&slotNum, pageData + numSlotsOff, sizeof(int));
            // cout << "slotNum:________: " << slotNum << endl;

            cout << "=======:==============: " << endl;

            // cout << "Page Data: ";
            // for (int i = 0; i < BLOCK_SIZE; ++i) {
            //     cout << hex << setw(2) << setfill('0') << (unsigned int)(unsigned char)pageData[i] << " ";
            //     if ((i + 1) % 16 == 0) // New line every 16 bytes for readability
            //         cout << endl;
            // }
            // cout << dec << endl; // Switch back to decimal format

            // int numSlotsOffset = BLOCK_SIZE - sizeof(int); // Calculate the offset for numSlots

            cout << "numSlotsOffset: " << numSlotsOff << endl;

            // Print the bytes in hex format
            // for (int i = numSlotsOff; i < BLOCK_SIZE; ++i) {
            //     unsigned char byte = static_cast<unsigned char>(pageData[i]);
            //     cout << hex << setw(2) << setfill('0') << static_cast<int>(byte) << " ";
            // }
            // cout << dec << endl; // Switch back to decimal format

            // Print the contents of pageData in hex format
            /*cout << "Page Data: ";
            for (int i = 0; i < BLOCK_SIZE; ++i)
            {
                char ch = pageData[i];
                if (isprint(ch))
                {
                    cout << ch; // Print as is if printable
                }
                else
                {
                    cout << "."; // Print a placeholder for non-printable characters
                }
            }
            cout << endl; // Switch back to decimal format

            cout << "INSIDE while loop" << endl;
            */
            // int numSlots = *reinterpret_cast<int*>(pageData + BLOCK_SIZE - sizeof(int));
            // int numSlots;
            // memcpy(&numSlots, pageData + BLOCK_SIZE - sizeof(int), sizeof(int));

            // cout << "numSlots: " << numSlots << endl;

            for (int i = 0; i < slotNum; ++i)
            {

                Slot slot = *reinterpret_cast<Slot *>(pageData + numSlotsOff - (i + 1) * sizeof(Slot));

                // if (slot.offset + slot.length > BLOCK_SIZE) {
                //     continue;  // Invalid slot, skip it
                // }

                // cout << "INSIDE for loop" << endl;

                cout << "Slot offset: " << slot.offset << ", Slot length: " << slot.length << endl;
                if (slot.offset < 0 || slot.offset + slot.length > BLOCK_SIZE)
                {
                    cerr << "Invalid slot detected." << endl;
                    continue; // Skip this slot
                }
                //get Record from File to compare ID
                string recordData(pageData + slot.offset, slot.length);

                cout << "INSIDE for loop" << endl;

                istringstream recStream(recordData);
                int recordId, managerId;
                string name, bio;

                //read record referring slot 
                recStream.read(reinterpret_cast<char *>(&recordId), sizeof(recordId));

                //if read record's id matches search id print the record
                if (recordId == id)
                {   
                    cout << "\n=============Printing Record with ID : "<< id <<" =============\n" ;
                    
                    getline(recStream, name, ',');
                    getline(recStream, bio, ',');
                    recStream.read(reinterpret_cast<char *>(&managerId), sizeof(managerId));
                    file.close();
                    return Record({to_string(recordId), name, bio, to_string(managerId)});
                }
            }
        }

        file.close();
        throw runtime_error("No record found with id: " + to_string(id));
    }
};
