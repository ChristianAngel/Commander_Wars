var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        var co = map.getCurrentPlayer().getCO(0);
        if ((co !== null) && co.canUsePower())
        {
            return true;
        }
        else
        {
            return false;
        }
    };
    this.getActionText = function()
    {
        var text = qsTr("Power of ");
        if (map !== null &&
            map.getCurrentPlayer() !== null &&
            map.getCurrentPlayer().getCO(0) !== null)
        {
            text = text + map.getCurrentPlayer().getCO(0).getCOName();
        }
        return text;
    };
    this.getIcon = function()
    {
        return "power";
    };
    this.perform = function(action)
    {
        map.getCurrentPlayer().getCO(0).activatePower();
        map.getGameRecorder().addSpecialEvent(map.getCurrentPlayer().getPlayerID(),
                                              GameEnums.GameRecord_SpecialEvents_Power);
    };
    this.isFinalStep = function(action)
    {
        return true;
    };
    this.getDescription = function()
    {
        return qsTr("Activates the Power of your first CO. Read the CO's power for more information.");
    };
}


Constructor.prototype = ACTION;
var ACTION_ACTIVATE_POWER_CO_0 = new Constructor();
