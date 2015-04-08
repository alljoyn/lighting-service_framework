/******************************************************************************
 * Copyright AllSeen Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for any
 *    purpose with or without fee is hereby granted, provided that the above
 *    copyright notice and this permission notice appear in all copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

#ifdef LSF_BINDINGS
#include <lsf/controllerservice/ControllerService.h>
#include <lsf/controllerservice/SceneElementManager.h>
#include <lsf/controllerservice/OEM_CS_Config.h>
#include <lsf/controllerservice/FileParser.h>
#else
#include <ControllerService.h>
#include <SceneElementManager.h>
#include <OEM_CS_Config.h>
#include <FileParser.h>
#endif

#include <qcc/atomic.h>
#include <qcc/Debug.h>

using namespace lsf;
using namespace ajn;

#ifdef LSF_BINDINGS
using namespace controllerservice;
#define QCC_MODULE "CONTROLLER_SCENE_ELEMENT_MANAGER"
#else
#define QCC_MODULE "SCENE_ELEMENT_MANAGER"
#endif

SceneElementManager::SceneElementManager(SceneManager* sceneMgr) :
    sceneManager(sceneMgr)
{
    QCC_DbgPrintf(("%s", __func__));
}

LSFResponseCode SceneElementManager::GetAllSceneElements(SceneElementMap& sceneElementMap)
{
    QCC_DbgTrace(("%s", __func__));
    LSFResponseCode responseCode = LSF_OK;

    return responseCode;
}

void SceneElementManager::GetAllSceneElementIDs(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));
}

void SceneElementManager::GetSceneElementName(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));
}

void SceneElementManager::SetSceneElementName(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));
}

void SceneElementManager::CreateSceneElement(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));
}

void SceneElementManager::UpdateSceneElement(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));
}

void SceneElementManager::DeleteSceneElement(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));
}

void SceneElementManager::GetSceneElement(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));
}

void SceneElementManager::ApplySceneElement(ajn::Message& message)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));
}

uint32_t SceneElementManager::GetControllerServiceSceneElementInterfaceVersion(void)
{
    QCC_DbgPrintf(("%s: controllerSceneElementInterfaceVersion=%d", __func__, ControllerServiceSceneElementInterfaceVersion));
    return ControllerServiceSceneElementInterfaceVersion;
}
