var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(0);
        co.setSuperpowerStars(0);
    };

    this.activatePower = function(co)
    {
    };

    this.activateSuperpower = function(co, powerMode)
    {
    };

    this.loadCOMusic = function(co)
    {
        // put the co music in here.
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/bh_power.mp3", 1091 , 49930);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/bh_superpower.mp3", 3161 , 37731);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/bh_tagpower.mp3", 779 , 51141);
                break;
            default:
                audio.addMusic("resources/music/cos/robosturm.mp3", 1395, 41092);
                break;
        }
    };

    this.getCOUnitRange = function(co)
    {
        return 0;
    };
    this.getCOArmy = function()
    {
        return "MA";
    };
    // CO - Intel
    this.getBio = function()
    {
        return qsTr("A Blank CO for Metal Army.") +
               qsTr("The Metall Army is an army of robots. The robots were initially invented by Lash. ") +
               qsTr("The Robots AI developped its own behaviour instead of following Lash's or Dark Matters orders. ") +
               qsTr("The Metall Army defends it's own territory at all costs when it looses it's territory. The Army tries to reconquer it.") +
               qsTr("Besides this the Metall Army is very defensive orientated and doesn't attack anyone.") ;
    };
    this.getHits = function()
    {
        return qsTr("N/A");
    };
    this.getMiss = function()
    {
        return qsTr("N/A");
    };
    this.getCODescription = function()
    {
        return qsTr("N/A");
    };
    this.getPowerDescription = function()
    {
        return qsTr("N/A");
    };
    this.getPowerName = function()
    {
        return qsTr("N/A");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("N/A");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("N/A");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("Attack!")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("Victory!")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("Defeat...")];
    };
    this.getName = function()
    {
        return qsTr("Metal Army");
    };
}

Constructor.prototype = CO;
var CO_EMPTY_MA = new Constructor();
