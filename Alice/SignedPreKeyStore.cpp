#include "SignedPreKeyStore.h"

int signed_pre_key_store_load_signed_pre_key(signal_buffer **record, uint32_t signed_pre_key_id, void *user_data)
{
    CriptextDB::Account *account = (CriptextDB::Account*)user_data;
    string dbPath(account->dbPath);
    CriptextDB::SignedPreKey signedPreKey;
    try {
        signedPreKey = CriptextDB::getSignedPreKey(dbPath, signed_pre_key_id);
    } catch (exception& e){
        std::cout << "ERROR SPK : " << e.what() << std::endl;
        return 0;
    }
    size_t len = 0;
    const unsigned char *recordBase64 = reinterpret_cast<const unsigned char *>(signedPreKey.record.c_str());
    uint8_t *myRecord = reinterpret_cast<uint8_t *>(base64_decode(recordBase64, signedPreKey.len, &len));    
    signal_buffer *buffer = signal_buffer_create(myRecord, len);

    *record = buffer;

    return 1;
}

int signed_pre_key_store_store_signed_pre_key(uint32_t signed_pre_key_id, uint8_t *record, size_t record_len, void *user_data)
{
    CriptextDB::Account *account = (CriptextDB::Account*)user_data;
    string dbPath(account->dbPath);

    size_t len = 0;
    const unsigned char *myRecord = reinterpret_cast<const unsigned char *>(record);
    char *recordBase64 = reinterpret_cast<char *>(base64_encode(myRecord, record_len, &len));

    bool success = CriptextDB::createSignedPreKey(dbPath, signed_pre_key_id, recordBase64, len);
    return success ? 1 : 0;
}

int signed_pre_key_store_contains_signed_pre_key(uint32_t signed_pre_key_id, void *user_data)
{
    CriptextDB::Account *account = (CriptextDB::Account*)user_data;
    string dbPath(account->dbPath);

    try {
        CriptextDB::getSignedPreKey(dbPath, signed_pre_key_id);
    } catch (exception& e){
        return 0;
    }
    return 1;
}

int signed_pre_key_store_remove_signed_pre_key(uint32_t signed_pre_key_id, void *user_data)
{
    CriptextDB::Account *account = (CriptextDB::Account*)user_data;
    string dbPath(account->dbPath);

    bool success = CriptextDB::deleteSignedPreKey(dbPath, signed_pre_key_id);
    return success ? 1 : 0;
}

void signed_pre_key_store_destroy(void *user_data)
{
    
}

void setup_signed_pre_key_store(signal_protocol_store_context *context, CriptextDB::Account *account)
{
    signal_protocol_signed_pre_key_store store = {
            signed_pre_key_store_load_signed_pre_key,
            signed_pre_key_store_store_signed_pre_key,
            signed_pre_key_store_contains_signed_pre_key,
            signed_pre_key_store_remove_signed_pre_key,
            signed_pre_key_store_destroy,
            account
    };

    signal_protocol_store_context_set_signed_pre_key_store(context, &store);
}