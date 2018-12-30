<!--

//Send Data
function sendPostRequest(iURL, iCmd, iFuntion) {
    var http = new XMLHttpRequest();
    var url = "SharedMem";
    if (null != iURL) {
        url = iURL;
    }
    http.open("POST", url, true);
    var params = JSON.stringify(iCmd);

    //Send the proper header information along with the request
    http.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    http.responseType = "arraybuffer"
    http.onreadystatechange = function () {//Call a function when the state changes.
        if (http.readyState == 4 && http.status == 200) {
            if (null != iFuntion) {

                var wResponse = http.response;
                iFuntion(wResponse);
            }
        }
    }

    http.send(params);
}


var gShareMemoryManager = null; 
var gShareMemoryOperator = null ; 

function getShareMemoryOperator()
{
    if (null == gShareMemoryOperator)
    {
        //        gShareMemoryOperator = new ShareMemoryOperator();
        gShareMemoryOperator = new ShareMemoryOperator_Remote();
    }
    return gShareMemoryOperator;
}

function refreshShareMemory()
{
    var wOperator = getShareMemoryOperator();

    if (null != wOperator)
    {
        wOperator.update();
    }
}

function getShareMemoryManager()
{
    if (null == gShareMemoryManager)
    {
        gShareMemoryManager = new ShareMemoryManager();
    }
    return gShareMemoryManager;
}

function getLabel(iLabel, iOffset)
{
    var wManager = getShareMemoryManager();
    if (null != wManager)
    {
        return wManager.getLabel(iLabel, iOffset);
    }
    return null;
}

function setLabel(iLabel, iValue, iOffset)
{
    var wManager = getShareMemoryManager();
    if (null != wManager)
    {
        return wManager.setLabel(iLabel, iValue, iOffset);
    }
    return false;
}


function shareMemUpdateAction( iDOM, iAction, iLabel, iOffset)
{
    var wManager = getShareMemoryManager();
    if (null != wManager)
    {
        switch(iAction)
        {
            case 0:
                {
                    var wData = wManager.getLabel(iLabel, iOffset);

                    if(null != wData)
                    {
                        //update value
                        iDOM.value = wData;
                    }
                    break;
                }
        }
    }
    return false;
}

function setLabelToDOMValue(iDOM, iLabel, iOffset)
{
    return shareMemUpdateAction(iDOM, 0, iLabel, iOffset)
}

var MemoryTypeEnum = Object.freeze(
{
    SInt8 : 1,
    SInt16 : 2,
    SInt32 : 3,
    UInt8 : 4,
    UInt16 : 5,
    UInt32 : 6,
    Float32 : 7,
    Float64 : 8,
    String8 : 9,
})

function getTypeSize(iType)
{
    switch(iType)
    {
        case MemoryTypeEnum.UInt8 :
        case MemoryTypeEnum.SInt8 :
        case MemoryTypeEnum.String8 :
            return 1;

        case MemoryTypeEnum.UInt16 :
        case MemoryTypeEnum.SInt16 :
            return 2;

        case MemoryTypeEnum.UInt32 :
        case MemoryTypeEnum.SInt32 :
        case MemoryTypeEnum.Float32 :
            return 4;

        case MemoryTypeEnum.Float64 :
            return 8;
        default:
            break;
    }

    return 0;
}

function ShareMemoryLabelEntry(iOffset, iType, iCount)
{
    this.Offset = iOffset;
    this.Type = iType;
    this.Count = iCount;
}

function QueueSetDataEntry(iLabel, iValue, iOffset)
{
    this.Label = iLabel;
    this.Value = iValue;
    this.Index = iOffset;
}

function ShareMemoryOperator()
{
    this.mBufferData = new ArrayBuffer(0);
    this.mBufferMap = new Array();
    this.mSetDataQueue = new Array();
    
    this.update = function() {

        this.sendSetData();
    }

    this.addLabel = function(iLabel, iType, iLength)
    {
        var wLabel = this.mBufferMap[iLabel];

        if (null == wLabel)
        {
            var wDataSize = getTypeSize(iType);

            if (0 != wDataSize)
            {
                var wLength = iLength;
                if ( 1 > wLength)
                {
                    wLength = 1;
                }

                var wBuffer = new ArrayBuffer(this.mBufferData.byteLength + wDataSize*wLength);

                for (var wi=0 ; wi < this.mBufferData.byteLength; ++wi){
                    wBuffer[wi] = this.mBufferData[wi];
                }
            
                this.mBufferMap[iLabel] = new ShareMemoryLabelEntry(this.mBufferData.byteLength, iType, wLength);
                this.mBufferData = wBuffer;
                this.pushArrayBufferMap();
                this.pushArrayBufferData();
                return true;
            }
        }
        return false;
    }
    
    this.setLabelValue = function(iLabel, iValue, iOffset)
    {
        this.mSetDataQueue.push(new QueueSetDataEntry(iLabel,iValue,iOffset));
        return true;
    }
    
    this.directSetLabelValue = function(iLabel, iValue, iOffset)
    {
        var wLabel = this.mBufferMap[iLabel];

        if (null != wLabel)
        {
            var wLocationOffset = 0;
            if (null != iOffset)
            {
                if (iOffset >= wLabel.Count)
                {
                    return false;
                }
                
                var wDataSize = getTypeSize(wLabel.Type);
                wLocationOffset = wDataSize*iOffset;
            }

            var wDataView = new DataView(this.mBufferData);
            var wComplete = false;
            switch(wLabel.Type)
            {
                case MemoryTypeEnum.UInt8 :
                    wDataView.setUint8(wLabel.Offset + wLocationOffset, iValue);
                    wComplete = true;
                    break;

                case MemoryTypeEnum.UInt16 :
                    wDataView.setUint16(wLabel.Offset + wLocationOffset, iValue);
                    wComplete = true;
                    break;

                case MemoryTypeEnum.UInt32 :
                    wDataView.setUint32(wLabel.Offset + wLocationOffset, iValue);
                    wComplete = true;
                    break;

                case MemoryTypeEnum.SInt8 :
                    wDataView.setInt8(wLabel.Offset + wLocationOffset, iValue);
                    wComplete = true;
                    break;

                case MemoryTypeEnum.SInt16 :
                    wDataView.setInt16(wLabel.Offset + wLocationOffset, iValue);
                    wComplete = true;
                    break;

                case MemoryTypeEnum.SInt32 :
                    wDataView.setInt32(wLabel.Offset + wLocationOffset, iValue);
                    wComplete = true;
                    break;

                case MemoryTypeEnum.Float32 :
                    wDataView.setFloat32(wLabel.Offset + wLocationOffset, iValue);
                    wComplete = true;
                    break;

                case MemoryTypeEnum.Float64 :
                    wDataView.setFloat64(wLabel.Offset + wLocationOffset, iValue);
                    wComplete = true;
                    break;
                    
                case MemoryTypeEnum.String8 :

                    for (var wi= wLocationOffset ; wi < wLabel.Count && wi < iValue.length; ++wi){
                        wDataView.setUint8(wLabel.Offset + wi, iValue.charCodeAt(wi));
                    }

                    for ( ; wi < wLabel.Count; ++wi)
                    {
                        wDataView.setUint8(wLabel.Offset + wi, 0);
                    }

                    wComplete = true;
                    break;

                default:
                    break;
            }

            if (true ==  wComplete)
            {
                return true;
            }

        }

        return false;
    }
    
    this.sendSetDataQueue = function()
    {
        var wQueue = this.mSetDataQueue;
        this.mSetDataQueue = new Array();

        for(var wi = 0; wi < wQueue.length; ++wi)
        {
            var wEntry = wQueue[wi];
            this.directSetLabelValue(wEntry.mLabel,wEntry.mValue,wEntry.mOffset);
        }
        
        this.pushArrayBufferData();
    }

    this.pushArrayBufferData = function()
    {
        var wShareMemory = getShareMemoryManager();

        if (null != wShareMemory)
        {
            wShareMemory.setShareMemoryBuffer(this.mBufferData);
        }
    }

    this.pushArrayBufferMap = function()
    {
        var wShareMemory = getShareMemoryManager();

        if (null != wShareMemory)
        {
            wShareMemory.setShareMemoryMap(this.mBufferMap)
        }
    }

    this.sendSetData = function()
    {
        this.sendSetDataQueue();
    }
}


function ShareMemoryOperator_Remote()
{
    this.mBufferVersion = 0;
    this.mBufferData = new ArrayBuffer(0);
    this.mBufferMap = {};
    this.mSetDataQueue = new Array();

    this.sendCmd = function (iObject, iCallback) {
        sendPostRequest("SharedMem", iObject, iCallback);
    }

    this.update = function() {

        this.sendSetData();

    }

    this.updateDataBufferReply = function (iByteString)
    {   
        var wPacketHeader = this.getPacketHeader(iByteString);

        if (null == wPacketHeader)
        {
            return;
        }

        if (1 != wPacketHeader.StringType)
        {
            return;
        }
        
        this.mBufferData = iByteString.slice(wPacketHeader.StringStart, wPacketHeader.StringStart + wPacketHeader.StringLength);

        if (this.mBufferVersion != wPacketHeader.MapVersion)
        {
            var wCmd = {
                cmd : "GetDataMap"
            }

            this.sendCmd(wCmd, this.updateDataMapReply.bind(this));
        }
        else
        {
            this.pushArrayBufferData();
        }
    }

    this.updateDataMapReply = function (iByteString)
    {
        var wPacketHeader = this.getPacketHeader(iByteString);

        if (null == wPacketHeader)
        {
            return;
        }

        if (3 != wPacketHeader.StringType)
        {
            return;
        }

        this.mBufferVersion = wPacketHeader.MapVersion


        var wCharString = new Uint8Array(iByteString.slice(wPacketHeader.StringStart, wPacketHeader.StringStart + wPacketHeader.StringLength));
        var wJSONString = String.fromCharCode.apply(null, wCharString);
        var wNewMap = JSON.parse(wJSONString);
        this.mBufferMap = wNewMap;

        
        this.pushArrayBufferData();
        this.pushArrayBufferMap();
    }

    this.getPacketHeader = function (iByteString)
    {
        //Verify Packet
        var wPacketName = "SHAREMEMRPLY";
        
        var wPCharSet = new Uint8Array(iByteString.slice(0, wPacketName.length));
        var wPString = String.fromCharCode.apply(null, wPCharSet);

        if(wPString != wPacketName)
        {
            return null;
        }

        var wUInt32Size = getTypeSize(MemoryTypeEnum.UInt32);
        var wDataView = new Uint32Array(iByteString.slice(0,8*wUInt32Size));

        var wi = 4;

        var wHeaderLength = wDataView[wi];
        ++wi;

        var wMapVersion = wDataView[wi];
        ++wi;

        var wStringType = wDataView[wi];
        ++wi;

        var wStringLength = wDataView[wi];
        ++wi;

        if (wi*wUInt32Size != wHeaderLength)
        {
            return null;
        }

        return {
            MapVersion : wMapVersion,
            StringType : wStringType,
            StringStart : wHeaderLength,
            StringLength : wStringLength
        };
    }
    
    this.addLabel = function(iLabel, iType, iLength)
    {
        return false;
    }

    this.setLabelValue = function(iLabel, iValue, iOffset)
    {
        this.mSetDataQueue.push(new QueueSetDataEntry(iLabel,iValue,iOffset));
        return true;
    }

    this.sendSetLabelValueInQueue = function()
    {
        var wQueue = this.mSetDataQueue;
        this.mSetDataQueue = new Array();

 
     //   if (0 != wQueue.length)
        {
            var wCmd = {
                cmd : "SetGetDataBlock",
                SetList : wQueue
            }
        
            this.sendCmd(wCmd, this.updateDataBufferReply.bind(this));
        }
    }

    this.pushArrayBufferData = function()
    {
        var wShareMemory = getShareMemoryManager();

        if (null != wShareMemory)
        {
            wShareMemory.setShareMemoryBuffer(this.mBufferData);
        }
    }

    this.pushArrayBufferMap = function()
    {
        var wShareMemory = getShareMemoryManager();

        if (null != wShareMemory)
        {
            wShareMemory.setShareMemoryMap(this.mBufferMap)
        }
    }
    
    this.sendSetData = function()
    {
        this.sendSetLabelValueInQueue();
    }
}

function ShareMemoryManager()
{
    this.mBufferData = null;
    this.mBufferMap = null;
    this.mDataSubscibers = new Array();
    
    this.setShareMemoryBuffer = function (iBufferData)
    {
        this.mBufferData = iBufferData.slice(0);
        this.updateSubscribers();
    }

    this.setShareMemoryMap = function(iBufferMap)
    {
        wBufferMap = new Array();
        
        for (var key in iBufferMap) {

            var wLabel = iBufferMap[key];
            if (null != wLabel)
            {
                wBufferMap[key] = new ShareMemoryLabelEntry(wLabel.Offset, wLabel.Type, wLabel.Count);
            }
        }
        
        this.mBufferMap = wBufferMap;
    }

    this.getLabel = function(iLabel, iOffset)
    {
        if (null == this.mBufferMap)
        {
            return null;
        }

        var wLabel = this.mBufferMap[iLabel];

        if (null != wLabel)
        {
            var wLocationOffset = 0;
            if (null != iOffset)
            {
                if (iOffset >= wLabel.Count)
                {
                    return null;
                }
                
                var wDataSize = getTypeSize(wLabel.Type);
                wLocationOffset = wDataSize*iOffset;
            }

            var wDataView = new DataView(this.mBufferData);
            switch(wLabel.Type)
            {
                case MemoryTypeEnum.UInt8 :
                    var wValue = wDataView.getUint8(wLabel.Offset + wLocationOffset, true);
                    return wValue;

                case MemoryTypeEnum.UInt16 :
                    return wDataView.getUint16(wLabel.Offset + wLocationOffset, true);

                case MemoryTypeEnum.UInt32 :
                    return wDataView.getUint32(wLabel.Offset + wLocationOffset, true);

                case MemoryTypeEnum.SInt8 :
                    return wDataView.getInt8(wLabel.Offset + wLocationOffset, true);

                case MemoryTypeEnum.SInt16 :
                    return wDataView.getInt16(wLabel.Offset + wLocationOffset, true);

                case MemoryTypeEnum.SInt32 :
                    return wDataView.getInt32(wLabel.Offset + wLocationOffset, true);

                case MemoryTypeEnum.Float32 :
                    return wDataView.getFloat32(wLabel.Offset + wLocationOffset, true);

                case MemoryTypeEnum.Float64 :
                    return wDataView.getFloat64(wLabel.Offset + wLocationOffset, true);

                case MemoryTypeEnum.String8 :
                    var wStart = wLabel.Offset + wLocationOffset;
                    var wPCharSet2 = new Uint8Array(this.mBufferData.slice(wStart, wStart + wLabel.Count));
                    var wPString2 = String.fromCharCode.apply(null, wPCharSet2);
                    
                    return wPString2.split('\0')[0];

                default:
                    return null;
            }

        }

        return null;
    }

    this.setLabel =  function (iLabel, iValue, iOffset)
    {
        var wOperator = getShareMemoryOperator();

        if (null != wOperator)
        {
            return wOperator.setLabelValue(iLabel, iValue, iOffset);
        }

        return false;
    }

    this.printBuffer = function()
    {
        var wOutputString = "Printing Buffer...\n";
        for (var key in this.mBufferMap) {

            wOutputString += key;
            wOutputString += " = ";

            var wLabel = this.mBufferMap[key];
            if (null != wLabel)
            {
                if (MemoryTypeEnum.String8 == wLabel.Type)
                {
                    wOutputString += this.getLabel(key);
                }
                else
                {
                    wOutputString += "[";
                    wOutputString += this.getLabel(key, 0);

                    for( var wi = 1; wi < wLabel.Count; ++wi)
                    {
                        wOutputString += ",";
                        wOutputString += this.getLabel(key, wi);               
                    }
                    wOutputString += "]";
                }
            }
            else
            {
                wOutputString += "null";
            }

            wOutputString+="\n";
        }
        
        return wOutputString;
    }

    this.updateSubscribers = function()
    {
        var wSubscribers = document.getElementsByClassName("ShareMemSubsciber");

        for(var wi = 0; wi < wSubscribers.length; ++wi)
        {
            var wElement = wSubscribers[wi];

            if (null != wElement)
            {
                var wAttribute = wElement.getAttribute('onShareMemUpdate');
                if (null != wAttribute)
                {
                    var wFunc = function(){
                        eval(wAttribute);
                    };
                    
                    var wBinded = wFunc.bind(wElement);

                    wBinded();
                }
            }
        }
    }
}

-->