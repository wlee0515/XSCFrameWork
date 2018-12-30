<!--
    
//Send Data
function sendPostRequest(iURL, iCmd, iFuntion) {
    var http = new XMLHttpRequest();
    var url = "CadClient";
    if (null != iURL) {
        url = iURL;
    }
    http.open("POST", url, true);
    var params = JSON.stringify(iCmd);

    //Send the proper header information along with the request
    http.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    http.onreadystatechange = function () {//Call a function when the state changes.
        if (http.readyState == 4 && http.status == 200) {
            if (null != iFuntion) {

                var wtxt = http.responseText;

                if ("" != wtxt) {
                    wtxt = wtxt.replace(/\u00B0/g, " ");
                    iFuntion(wtxt);
                }
            }
        }
    }

    http.send(params);
}

function updateArrayToSelection(iSelectionID, iArray) {
    var wContainer = document.getElementById(iSelectionID);
    if (null != wContainer) {
        if (null != iArray) {
            
            var wContainerChildren = wContainer.getElementsByClassName(iSelectionID + "_select" );

            
            if ( wContainerChildren.length > iArray.length)
            {
                var wNumberToRemove = wContainerChildren.length - iArray.length;

                for ( var wi = 0; wi < wNumberToRemove; ++wi) {
                    wContainer.removeChild( wContainerChildren[wContainerChildren.length - 1]);
                }
            }
        
            if ( wContainerChildren.length < iArray.length)
            {
                var wNumberToAdd = iArray.length - wContainerChildren.length;
            
                for ( var wi = 0; wi < wNumberToAdd; ++wi) {
                    var wNewRow = document.createElement("option");
                    wNewRow.className = iSelectionID  + "_select" ;
                    wContainer.appendChild(wNewRow);
                }
            }

            
            var wNewList = wContainer.getElementsByClassName(iSelectionID + "_select" );;

            for ( var wi = 0; wi < wNewList.length; ++ wi)
            {
                if (wNewList[wi].value != iArray[wi])
                {
                    wNewList[wi].value = iArray[wi];
                    wNewList[wi].innerText = iArray[wi];
                }
            }
        }
    }
}


function findSelectionIndex(iSelectionID, iSelectionValue) {
    var wSelectionIndex = -1;
    var wContainer = document.getElementById(iSelectionID);
    if (null != wContainer) {

        var wContainerChildren = wContainer.getElementsByClassName(iSelectionID + "_select" );

        for ( var wi = 0; wi < wContainerChildren.length; ++ wi) {
            if (wContainerChildren[wi].value == iSelectionValue) {
                return wi;
            }
        }
        return wSelectionIndex;
    }
}

function updateArrayToDivTable (iContentContainerId, iArrayValue, onClickCall ) {
    
    var wContainer = document.getElementById(iContentContainerId);

    if (null != wContainer)
    {
        var wContainerChildren = wContainer.getElementsByClassName(iContentContainerId + "_tr" );

        if ( wContainerChildren.length > iArrayValue.length)
        {
            var wNumberToRemove = wContainerChildren.length - iArrayValue.length;

            for ( var wi = 0; wi < wNumberToRemove; ++wi) {
                wContainer.removeChild( wContainerChildren[wContainerChildren.length - 1]);
            }
        }
        
        if ( wContainerChildren.length < iArrayValue.length)
        {
            var wNumberToAdd = iArrayValue.length - wContainerChildren.length;
            
            for ( var wi = 0; wi < wNumberToAdd; ++wi) {
                var wNewRow = document.createElement("div");
                wNewRow.className = iContentContainerId + "_tr" + " NotSelected";
                wNewRow.divTableIndex = wContainerChildren.length;
            
                wNewRow.addEventListener("mouseup", divTableCallback);
                if ((null != onClickCall) && ("" != onClickCall ))
                {
                    wNewRow.addEventListener("mouseup", onClickCall);
                }
                wContainer.appendChild(wNewRow);
            }
        }
        
        var wNewList = wContainer.getElementsByClassName(iContentContainerId + "_tr" );;

        for ( var wi = 0; wi < wNewList.length; ++ wi)
        {
            var wCurrentRow =  wNewList[wi];
            var wDataList = wCurrentRow.getElementsByClassName(iContentContainerId + "_td" );
            
            if ( wDataList.length > iArrayValue[wi].length)
            {
                var wNumberToRemove = wDataList.length - iArrayValue[wi].length;
                for ( var wj = 0; wj < wNumberToRemove; ++wj) {
                    wCurrentRow.removeChild( wDataList[wDataList.length - 1]);
                }
            }
        
            if ( wDataList.length < iArrayValue[wi].length)
            {
                var wNumberToAdd = iArrayValue[wi].length - wDataList.length;
                for ( var wj = 0; wj < wNumberToAdd; ++wj) {
                
                    var wNewCell = document.createElement("div");
                    wNewCell.className = iContentContainerId + "_td td_" + (wj + 1);
                    wNewCell.style.display = "inline-block";
                    wCurrentRow.appendChild(wNewCell);
                }
            }
            
            var wDataList = wCurrentRow.getElementsByClassName(iContentContainerId + "_td" );
            for ( var wj = 0; wj < iArrayValue[wi].length; ++wj)
            {
                if (wDataList[wj].innerText != iArrayValue[wi][wj])
                {
                    wDataList[wj].innerText = iArrayValue[wi][wj];
                }
            }
        }
    }
}

function divTableCallback(iEvt)
{
    var wDOM = this;
    var wContainer = wDOM.parentNode;
        
    if (null != wContainer)
    {
        var wContainerChildren = wContainer.getElementsByClassName(wContainer.id + "_tr" );
            
        for ( var wi = 0; wi < wContainerChildren.length; ++ wi)
        {
            if ( wDOM == wContainerChildren[wi])
            {
                wContainerChildren[wi].className = wContainer.id + "_tr Selected";
            }
            else
            {
                wContainerChildren[wi].className = wContainer.id + "_tr NotSelected";
            }
        }
    }
}

function getSelectedDivTableDataArray(iContentContainerId)
{
        
    var wContainer = document.getElementById(iContentContainerId);

    if (null != wContainer)
    {
        var wSelectedRow = wContainer.getElementsByClassName(iContentContainerId + "_tr Selected" );

        if (null != wSelectedRow[0])
        {
            var wDataList = wSelectedRow[0].getElementsByClassName(wContainer.id + "_td" );
                
            var wOutput = new Array();
            for ( var wi = 0; wi < wDataList.length; ++ wi)
            {
                wOutput.push(wDataList[wi].innerText);
            }
            return wOutput;
        }
    }

    return null;
}
	
function getSelectedDivTableIndex(iContentContainerId)
{
        
    var wContainer = document.getElementById(iContentContainerId);

    if (null != wContainer)
    {
        var wSelectedRow = wContainer.getElementsByClassName(iContentContainerId + "_tr Selected" );

        if (null != wSelectedRow[0])
        {
            if (null != wSelectedRow[0].divTableIndex)
            {
                return wSelectedRow[0].divTableIndex;
            }
        }
    }

    return -1;
}


function updateArrayToDivTree (iContentContainerId, iArrayValue, onClickCall ) {
    
    var wContainer = document.getElementById(iContentContainerId);

    if (null != wContainer)
    {
        //Preprocess Tree
        var wTreeStructure = new Array();
        for ( var wi = 0; wi < iArrayValue.length; ++ wi)
        {
            var wStringArr = iArrayValue[wi][0].split("]");
            var wBarArray = new Array();
            
            for(var wk = 0; wk < wStringArr.length - 1; ++wk)
            {
                wBarArray.push(true);
            }

            wTreeStructure.push( {
                mBareArray : wBarArray,
                mValue :  iArrayValue[wi],
                mText0  :  wStringArr[wStringArr.length - 1]
            });


        }

        var wLastIndex = wTreeStructure.length - 1;
        for(var wk = 0; wk < wTreeStructure[wLastIndex].mBareArray.length - 1; ++wk)
        {
            wTreeStructure[wLastIndex].mBareArray[wk] = false;
        }

        for ( var wi = wTreeStructure.length - 2; wi >= 0; --wi)
        {
            for(var wk = 0; wk < wTreeStructure[wi].mBareArray.length-1; ++wk)
            {
                if ( wk >= wTreeStructure[wi+1].mBareArray.length)
                {
                    wTreeStructure[wi].mBareArray[wk] = false;
                }
                else if(false == wTreeStructure[wi+1].mBareArray[wk])
                {
                    wTreeStructure[wi].mBareArray[wk] = false;
                }
            }
        }




        var wContainerChildren = wContainer.getElementsByClassName(iContentContainerId + "_tr" );

        if ( wContainerChildren.length > wTreeStructure.length)
        {
            var wNumberToRemove = wContainerChildren.length - wTreeStructure.length;

            for ( var wi = 0; wi < wNumberToRemove; ++wi) {
                wContainer.removeChild( wContainerChildren[wContainerChildren.length - 1]);
            }
        }
        
        if ( wContainerChildren.length < wTreeStructure.length)
        {
            var wNumberToAdd = wTreeStructure.length - wContainerChildren.length;
            
            for ( var wi = 0; wi < wNumberToAdd; ++wi) {
                var wNewRow = document.createElement("div");
                wNewRow.className = iContentContainerId + "_tr" + " NotSelected";
                wNewRow.divTreeIndex = wContainerChildren.length;
            
                wNewRow.addEventListener("mouseup", divTreeCallback);
                if ((null != onClickCall) && ("" != onClickCall ))
                {
                    wNewRow.addEventListener("mouseup", onClickCall);
                }
                wContainer.appendChild(wNewRow);
            }
        }

        var wNewList = wContainer.getElementsByClassName(iContentContainerId + "_tr" );;

        for ( var wi = 0; wi < wNewList.length; ++ wi)
        {
            var wCurrentRow =  wNewList[wi];
            var wDataList = wCurrentRow.getElementsByClassName(iContentContainerId + "_td" );
            
            if ( wDataList.length > wTreeStructure[wi].mValue.length)
            {
                var wNumberToRemove = wDataList.length - wTreeStructure[wi].mValue.length;
                for ( var wj = 0; wj < wNumberToRemove; ++wj) {
                    wCurrentRow.removeChild( wDataList[wDataList.length - 1]);
                }
            }
        
            if ( wDataList.length < wTreeStructure[wi].mValue.length)
            {
                var wNumberToAdd = wTreeStructure[wi].mValue.length - wDataList.length;
                for ( var wj = 0; wj < wNumberToAdd; ++wj) {
                
                    var wNewCell = document.createElement("div");
                    wNewCell.className = iContentContainerId + "_td td_" + (wj + 1);
                    wNewCell.style.display = "inline-block";
                    wCurrentRow.appendChild(wNewCell);
                }
            }
            
            var wDataList = wCurrentRow.getElementsByClassName(iContentContainerId + "_td" );
            for ( var wj = 0; wj < wTreeStructure[wi].mValue.length; ++wj)
            {
                var wUpdate = false;
                if (null == wDataList[wj].value)
                {
                    wUpdate = true;
                }
                else if (wDataList[wj].value != wTreeStructure[wi].mValue[wj])
                {
                    wUpdate = true;

                }

                if (true == wUpdate)
                {
                    wDataList[wj].value = wTreeStructure[wi].mValue[wj];
                    
                    if (0 == wj)
                    {
                        
                        var wOutputString = "";

                        if ( 0 != wTreeStructure[wi].mBareArray.length)
                        {
                            for(var wk = 0; wk < wTreeStructure[wi].mBareArray.length-1; ++wk)
                            {
                                if (true == wTreeStructure[wi].mBareArray[wk])
                                {
                                    wOutputString +="\xa0|\xa0";
                                }
                                else
                                {
                                    wOutputString +="\xa0\xa0\xa0";
                                }
                            }

                            wOutputString +="\xa0|_";
                        }
                        wOutputString += wTreeStructure[wi].mText0;
                        wDataList[wj].innerText = wOutputString;
                    }
                    else
                    {
                        wDataList[wj].innerText = iArrayValue[wi][wj];
                    }
                }
            }
        }
    }
}

function divTreeCallback(iEvt)
{
    var wDOM = this;
    var wContainer = wDOM.parentNode;
        
    if (null != wContainer)
    {
        var wContainerChildren = wContainer.getElementsByClassName(wContainer.id + "_tr" );
            
        for ( var wi = 0; wi < wContainerChildren.length; ++ wi)
        {
            if ( wDOM == wContainerChildren[wi])
            {
                wContainerChildren[wi].className = wContainer.id + "_tr Selected";
            }
            else
            {
                wContainerChildren[wi].className = wContainer.id + "_tr NotSelected";
            }
        }
    }
}

function getSelectedDivTreeDataArray(iContentContainerId)
{
        
    var wContainer = document.getElementById(iContentContainerId);

    if (null != wContainer)
    {
        var wSelectedRow = wContainer.getElementsByClassName(iContentContainerId + "_tr Selected" );

        if (null != wSelectedRow[0])
        {
            var wDataList = wSelectedRow[0].getElementsByClassName(wContainer.id + "_td" );
                
            var wOutput = new Array();
            for ( var wi = 0; wi < wDataList.length; ++ wi)
            {
                wOutput.push(wDataList[wi].value);
            }
            return wOutput;
        }
    }

    return null;
}


function getSelectedDivTreeIndex(iContentContainerId)
{
        
    var wContainer = document.getElementById(iContentContainerId);

    if (null != wContainer)
    {
        var wSelectedRow = wContainer.getElementsByClassName(iContentContainerId + "_tr Selected" );

        if (null != wSelectedRow[0])
        {
            if (null != wSelectedRow[0].divTreeIndex)
            {
                return wSelectedRow[0].divTreeIndex;
            }
        }

    }

    return -1;
}
-->