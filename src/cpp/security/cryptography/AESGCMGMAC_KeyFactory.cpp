// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 * @file AESGCMGMAC_KeyFactory.cpp
 */

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>

#include "AESGCMGMAC_KeyFactory.h"

using namespace eprosima::fastrtps::rtps::security;

AESGCMGMAC_KeyFactory::AESGCMGMAC_KeyFactory(){}

ParticipantCryptoHandle * AESGCMGMAC_KeyFactory::register_local_participant(
                const IdentityHandle &participant_identity, 
                const PermissionsHandle &participant_permissions, 
                const PropertySeq &participant_properties, 
                SecurityException &exception){

    AESGCMGMAC_ParticipantCryptoHandle* PCrypto = nullptr;
    if( (!participant_identity.nil()) | (!participant_permissions.nil()) ){
        exception = SecurityException("Invalid input parameters");
        return nullptr;
    }

    PCrypto = new AESGCMGMAC_ParticipantCryptoHandle();
    (*PCrypto)->ParticipantKeyMaterial = new KeyMaterial_AES_GCM_GMAC();
    //Fill CryptoData
    (*PCrypto)->ParticipantKeyMaterial->transformation_kind = 
            std::array<uint8_t,4>(CRYPTO_TRANSFORMATION_KIND_AES128_GCM);
    (*PCrypto)->ParticipantKeyMaterial->master_salt.fill(0);
    RAND_bytes( (*PCrypto)->ParticipantKeyMaterial->master_salt.data(), 128 );  
    (*PCrypto)->ParticipantKeyMaterial->sender_key_id = make_unique_KeyId();
    (*PCrypto)->ParticipantKeyMaterial->master_sender_key.fill(0);
    RAND_bytes( (*PCrypto)->ParticipantKeyMaterial->master_sender_key.data(), 128 );
    (*PCrypto)->ParticipantKeyMaterial->receiver_specific_key_id = make_unique_KeyId();
    (*PCrypto)->ParticipantKeyMaterial->master_receiver_specific_key.fill(0); 
    RAND_bytes( (*PCrypto)->ParticipantKeyMaterial->master_receiver_specific_key.data(), 128 );
    return PCrypto;
}
        
ParticipantCryptoHandle * AESGCMGMAC_KeyFactory::register_matched_remote_participant(
                ParticipantCryptoHandle &local_participant_crypto_handle, 
                IdentityHandle &remote_participant_identity, 
                PermissionsHandle &remote_participant_permissions, 
                SharedSecretHandle &shared_secret, 
                SecurityException &exception){

    AESGCMGMAC_ParticipantCryptoHandle& local_participant_handle = AESGCMGMAC_ParticipantCryptoHandle::narrow(local_participant_crypto_handle);
    AESGCMGMAC_ParticipantCryptoHandle* RPCrypto = nullptr;
    if( (!remote_participant_identity.nil()) | (!remote_participant_permissions.nil()) ){
        exception = SecurityException("Invalid input parameters");
        return nullptr;
    }
    
    RPCrypto = new AESGCMGMAC_ParticipantCryptoHandle();
    
    (*RPCrypto)->Participant2ParticipantKeyMaterial = new KeyMaterial_AES_GCM_GMAC(); 
    (*RPCrypto)->Participant2ParticipantKxKeyMaterial = new KeyMaterial_AES_GCM_GMAC();
    //Fill CryptoData - Participant2ParticipantKeyMaterial
    (*RPCrypto)->Participant2ParticipantKeyMaterial->transformation_kind = local_participant_handle->ParticipantKeyMaterial->transformation_kind;
    (*RPCrypto)->Participant2ParticipantKeyMaterial->master_salt = local_participant_handle->ParticipantKeyMaterial->master_salt;
    (*RPCrypto)->Participant2ParticipantKeyMaterial->master_sender_key = local_participant_handle->ParticipantKeyMaterial->master_sender_key;
    //Fill CryptoData - Participant2ParticipantKxKeymaterial
    (*RPCrypto)->Participant2ParticipantKxKeyMaterial->transformation_kind = std::array<uint8_t,4>(CRYPTO_TRANSFORMATION_KIND_AES128_GMAC);
    (*RPCrypto)->Participant2ParticipantKxKeyMaterial->master_salt.fill(0);
    RAND_bytes( (*RPCrypto)->Participant2ParticipantKxKeyMaterial->master_salt.data(), 128); // To substitute with HMAC_sha
    (*RPCrypto)->Participant2ParticipantKxKeyMaterial->sender_key_id.fill(0);
    (*RPCrypto)->Participant2ParticipantKxKeyMaterial->master_sender_key.fill(0);
    RAND_bytes( (*RPCrypto)->Participant2ParticipantKxKeyMaterial->master_sender_key.data(), 128); // To substitute with HMAC_sha
    (*RPCrypto)->Participant2ParticipantKxKeyMaterial->receiver_specific_key_id.fill(0);
    (*RPCrypto)->Participant2ParticipantKxKeyMaterial->master_receiver_specific_key.fill(0);
    return RPCrypto;
}

DatawriterCryptoHandle * AESGCMGMAC_KeyFactory::register_local_datawriter(
                const ParticipantCryptoHandle &participant_crypto,
                const PropertySeq &datawriter_prop,
                SecurityException &exception){

    exception = SecurityException("Not implemented");
    return nullptr;
}

DatareaderCryptoHandle * AESGCMGMAC_KeyFactory::register_matched_remote_datareader(
                const DatawriterCryptoHandle &local_datawriter_crypto_handle,
                const ParticipantCryptoHandle &lremote_participant_crypto,
                const SharedSecretHandle &shared_secret,
                const bool relay_only,
                SecurityException &exception){

    exception = SecurityException("Not implemented");
    return nullptr;
}

DatareaderCryptoHandle * AESGCMGMAC_KeyFactory::register_local_datareader(
                const ParticipantCryptoHandle &participant_crypto,
                const PropertySeq &datareader_properties,
                SecurityException &exception){

    exception = SecurityException("Not implemented");
    return nullptr;
}

DatawriterCryptoHandle * AESGCMGMAC_KeyFactory::register_matched_remote_datawriter(
                const DatareaderCryptoHandle &local_datareader_crypto_handle,
                const ParticipantCryptoHandle &remote_participant_crypt,
                const SharedSecretHandle &shared_secret,
                SecurityException &exception){

    exception = SecurityException("Not implemented");
    return nullptr;
}

bool AESGCMGMAC_KeyFactory::unregister_participant(
                const ParticipantCryptoHandle &participant_crypto_handle,
                SecurityException &exception){

    exception = SecurityException("Not implemented");
    return false;
}
        
bool AESGCMGMAC_KeyFactory::unregister_datawriter(
                const DatawriterCryptoHandle &datawriter_crypto_handle,
                SecurityException &exception){

    exception = SecurityException("Not implemented");
    return false;
}

        
bool AESGCMGMAC_KeyFactory::unregister_datareader(
                const DatareaderCryptoHandle &datareader_crypto_handle,
                SecurityException &exception){

    exception = SecurityException("Not implemented");
    return false;
}

CryptoTransformKeyId AESGCMGMAC_KeyFactory::make_unique_KeyId(){
    CryptoTransformKeyId buffer;
    bool unique = false;

    while(!unique){
        RAND_bytes(buffer.data(),4);
        unique = true;
        //Iterate existing KeyIds to see if one is matching
        for(std::vector<CryptoTransformKeyId>::iterator it=m_CryptoTransformKeyIds.begin(); it!=m_CryptoTransformKeyIds.end();it++){
            if(*it == buffer)   unique = false;
        }
    }
    return buffer;
}
