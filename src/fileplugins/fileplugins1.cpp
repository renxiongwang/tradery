/*
Copyright (C) 2018 Adrian Michel
http://www.amichel.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "stdafx.h"
#include "fileplugins1.h"
#include "slippage.h"
#include "commission.h"
#include <statsdefines.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
BEGIN_MESSAGE_MAP(Cfileplugins1App, CWinApp)
END_MESSAGE_MAP()

FileDataSourcePlugin dataSourcePlugin;
FileSymbolsSourcePlugin symbolsSourcePlugin;
FileStatsPlugin statsPlugin;
SimpleCommissionPlugin cplugin;
SimpleSlippagePlugin splugin;

PLUGIN_API tradery::SlippagePlugin* getSlippagePlugin() { return &splugin; }

PLUGIN_API tradery::CommissionPlugin* getCommissionPlugin() { return &cplugin; }

// Cfileplugins1App construction

Cfileplugins1App::Cfileplugins1App() {
  // TODO: add construction code here,
  // Place all significant initialization in InitInstance
}

// The one and only Cfileplugins1App object

Cfileplugins1App theApp;

// Cfileplugins1App initialization

BOOL Cfileplugins1App::InitInstance() {
  CWinApp::InitInstance();
  cplugin.init();
  splugin.init();

  return TRUE;
}

PLUGIN_API tradery::DataSourcePlugin* getDataSourcePlugin() {
  return &dataSourcePlugin;
}

// gets the symbols source plugin
PLUGIN_API tradery::SymbolsSourcePlugin* getSymbolsSourcePlugin() {
  return &symbolsSourcePlugin;
}

PLUGIN_API tradery::SignalHandlerPlugin* getSignalHandlerPlugin() {
  //  std::cout << "getting stats plugin" << std::endl;
  return &statsPlugin;
}

void FileStatsHandler::toFormat(const StatsToFormat& format) const {
  LOG(log_debug, "1");
  format.header(totalStats().dateRange());

  LOG(log_debug, "2");
  size_t size = equityCurve().size();
  LOG(log_debug, "3");
  const DrawdownCurve& totalDC(__super::totalDrawdownCurve());
  LOG(log_debug, "4");
  const DrawdownCurve& longDC(__super::longDrawdownCurve());
  LOG(log_debug, "5");
  const DrawdownCurve& shortDC(__super::shortDrawdownCurve());
  LOG(log_debug, "6");
  size_t bhsize = bhEquityCurve().size();
  const DrawdownCurve& bhDC(__super::bhDrawdownCurve());

  LOG(log_debug, "Getting b&h drawdown max value: " << bhDC.maxValue());
  LOG(log_debug, "After getting the b&h dd max value");

  format.subtitle(OVERALL_PERFORMANCE);
  format.row(TRADERY_SCORE, totalStats().getScore(), longStats().getScore(),
             shortStats().getScore(), bhStats().getScore());
  format.subtitle(PERFORMANCE);
  format.row(TOTAL_PCT_GAIN_LOSS, totalStats().allPosStats().pctGainLoss(),
             longStats().allPosStats().pctGainLoss(),
             shortStats().allPosStats().pctGainLoss(),
             bhStats().allPosStats().pctGainLoss(), true, true);
  format.row(TOTAL_GAIN_LOSS, totalStats().allPosStats().gainLoss(),
             longStats().allPosStats().gainLoss(),
             shortStats().allPosStats().gainLoss(),
             bhStats().allPosStats().gainLoss());
  format.row(ANNUALIZED_PCT_GAIN,
             totalStats().allPosStats().annualizedPctGain(),
             longStats().allPosStats().annualizedPctGain(),
             shortStats().allPosStats().annualizedPctGain(),
             bhStats().allPosStats().annualizedPctGain(), true, true);

  format.row(STARTING_CAPITAL, totalStats().allPosStats().initialCapital(),
             longStats().allPosStats().initialCapital(),
             shortStats().allPosStats().initialCapital(),
             bhStats().allPosStats().initialCapital(), false);

  format.row(ENDING_CAPITAL, totalStats().allPosStats().endingCapital(),
             longStats().allPosStats().endingCapital(),
             shortStats().allPosStats().endingCapital(),
             bhStats().allPosStats().endingCapital());

  format.row(CLOSE_POS_GAIN_LOSS, totalStats().closePosStats().gainLoss(),
             longStats().closePosStats().gainLoss(),
             shortStats().closePosStats().gainLoss(),
             bhStats().closePosStats().gainLoss());
  format.row(OPEN_POS_GAIN_LOSS, totalStats().openPosStats().gainLoss(),
             longStats().openPosStats().gainLoss(),
             shortStats().openPosStats().gainLoss(),
             bhStats().openPosStats().gainLoss());
  format.row(MAX_GAIN_PER_POS, totalStats().allPosStats().maxGainPerPos(),
             longStats().allPosStats().maxGainPerPos(),
             shortStats().allPosStats().maxGainPerPos(),
             bhStats().allPosStats().maxGainPerPos());
  format.row(MAX_LOSS_PER_POS, totalStats().allPosStats().maxLossPerPos(),
             longStats().allPosStats().maxLossPerPos(),
             shortStats().allPosStats().maxLossPerPos(),
             bhStats().allPosStats().maxLossPerPos());
  format.row(AVERAGE_GAIN_LOSS_PER_POS,
             totalStats().allPosStats().averageGainLossPerPos(),
             longStats().allPosStats().averageGainLossPerPos(),
             shortStats().allPosStats().averageGainLossPerPos(),
             bhStats().allPosStats().averageGainLossPerPos());
  format.row(AVERAGE_PCT_GAIN_LOSS_PER_POS,
             totalStats().allPosStats().averagePctGainLossPerPos(),
             longStats().allPosStats().averagePctGainLossPerPos(),
             shortStats().allPosStats().averagePctGainLossPerPos(),
             bhStats().allPosStats().averagePctGainLossPerPos(), true, true);
  format.row(MAX_PCT_GAIN_PER_POS,
             totalStats().allPosStats().maxPctGainPerPos(),
             longStats().allPosStats().maxPctGainPerPos(),
             shortStats().allPosStats().maxPctGainPerPos(),
             bhStats().allPosStats().maxPctGainPerPos(), true, true);
  format.row(MAX_PCT_LOSS_PER_POS,
             totalStats().allPosStats().maxPctLossPerPos(),
             longStats().allPosStats().maxPctLossPerPos(),
             shortStats().allPosStats().maxPctLossPerPos(),
             bhStats().allPosStats().maxPctLossPerPos(), true, true);

  format.subtitle(RISK_ETC);
  format.row(EXPOSURE, equityCurve().getTotalPctExposure(),
             equityCurve().getLongPctExposure(),
             equityCurve().getShortPctExposure(),
             bhEquityCurve().getTotalPctExposure(), true, true);
  format.row(EXPECTANCY, totalStats().allPosStats().expectancy(),
             longStats().allPosStats().expectancy(),
             shortStats().allPosStats().expectancy(),
             bhStats().allPosStats().expectancy());
  format.row(ULCER_INDEX, totalDC.ulcerIndex(), longDC.ulcerIndex(),
             shortDC.ulcerIndex(), bhDC.ulcerIndex());

  format.subtitle(DRAWDOWN);
  format.row(MAX_DRAWDOWN, totalDC.maxValue(), longDC.maxValue(),
             shortDC.maxValue(), bhDC.maxValue());
  format.row(MAX_DRAWDOWN_DATE, totalDC.maxDate(), longDC.maxDate(),
             shortDC.maxDate(), bhDC.maxDate());
  format.row(MAX_DRAWDOWN_PCT, totalDC.maxPct(), longDC.maxPct(),
             shortDC.maxPct(), bhDC.maxPct(), true, true);
  format.row(MAX_DRAWDOWN_PCT_DATE, totalDC.maxPctDate(), longDC.maxPctDate(),
             shortDC.maxPctDate(), bhDC.maxPctDate());
  format.row(MAX_DRAWDOWN_DURATION, totalDC.maxDrawdownDays(),
             longDC.maxDrawdownDays(), shortDC.maxDrawdownDays(),
             bhDC.maxDrawdownDays(), true, false, 0);

  format.subtitle(ALL_TRADES);
  format.row(TOTAL_TRADES, totalStats().allPosStats().count(),
             longStats().allPosStats().count(),
             shortStats().allPosStats().count(),
             bhStats().allPosStats().count(), true, false, 0);
  format.row(CLOSED_TRADES, totalStats().closePosStats().count(),
             longStats().closePosStats().count(),
             shortStats().closePosStats().count(),
             bhStats().closePosStats().count(), true, false, 0);
  format.row(OPEN_TRADES, totalStats().openPosStats().count(),
             longStats().openPosStats().count(),
             shortStats().openPosStats().count(),
             bhStats().openPosStats().count(), true, false, 0);

  format.subtitle(WINNING_TRADES);
  format.row(WINNING_CLOSED_TRADES, totalStats().closePosStats().winningCount(),
             longStats().closePosStats().winningCount(),
             shortStats().closePosStats().winningCount(),
             bhStats().closePosStats().winningCount(), true, false, 0);
  format.row(WINNING_OPEN_TRADES, totalStats().openPosStats().winningCount(),
             longStats().openPosStats().winningCount(),
             shortStats().openPosStats().winningCount(),
             bhStats().openPosStats().winningCount(), true, false, 0);
  format.row(PCT_WINNING_CLOSED_TRADES, totalStats().pctWinningClosed(),
             longStats().pctWinningClosed(), shortStats().pctWinningClosed(),
             bhStats().pctWinningClosed(), true, true);
  format.row(PCT_WINNING_OPEN_TRADES, totalStats().pctWinningOpen(),
             longStats().pctWinningOpen(), shortStats().pctWinningOpen(),
             bhStats().pctWinningOpen(), true, true);
  format.row(AVERAGE_GAIN_PER_WINNING_POS,
             totalStats().allPosStats().averageGainPerWinningPos(),
             longStats().allPosStats().averageGainPerWinningPos(),
             shortStats().allPosStats().averageGainPerWinningPos(),
             bhStats().allPosStats().averageGainPerWinningPos());

  format.subtitle(LOSING_TRADES);
  format.row(LOSING_CLOSED_TRADES, totalStats().closePosStats().losingCount(),
             longStats().closePosStats().losingCount(),
             shortStats().closePosStats().losingCount(),
             bhStats().closePosStats().losingCount(), true, false, 0);
  format.row(LOSING_OPEN_TRDES, totalStats().openPosStats().losingCount(),
             longStats().openPosStats().losingCount(),
             shortStats().openPosStats().losingCount(),
             bhStats().openPosStats().losingCount(), true, false, 0);
  format.row(PCT_LOSING_CLOSED_TRADES, totalStats().pctLosingClosed(),
             longStats().pctLosingClosed(), shortStats().pctLosingClosed(),
             bhStats().pctLosingClosed(), true, true);
  format.row(PCT_LOSING_OPEN_TRADES, totalStats().pctLosingOpen(),
             longStats().pctLosingOpen(), shortStats().pctLosingOpen(),
             bhStats().pctLosingOpen(), true, true);
  format.row(AVARAGE_LOSS_PER_LOSING_POS,
             totalStats().allPosStats().averageLossPerLosingPos(),
             longStats().allPosStats().averageLossPerLosingPos(),
             shortStats().allPosStats().averageLossPerLosingPos(),
             bhStats().allPosStats().averageLossPerLosingPos());

  format.subtitle(NEUTRAL_TRADES);
  format.row(NEUTRAL_CLOSED_TRADES, totalStats().closePosStats().neutralCount(),
             longStats().closePosStats().neutralCount(),
             shortStats().closePosStats().neutralCount(),
             bhStats().closePosStats().neutralCount(), true, false, 0);
  format.row(NEUTRAL_OPEN_TRADES, totalStats().openPosStats().neutralCount(),
             longStats().openPosStats().neutralCount(),
             shortStats().openPosStats().neutralCount(),
             bhStats().openPosStats().neutralCount(), true, false, 0);
  format.row(PCT_NEUTRAL_CLOSED_TRADES, totalStats().pctNeutralClosed(),
             longStats().pctNeutralClosed(), shortStats().pctNeutralClosed(),
             bhStats().pctNeutralClosed(), true, true);
  format.row(PCT_NEUTRAL_OPEN_TRADES, totalStats().pctNeutralOpen(),
             longStats().pctNeutralOpen(), shortStats().pctNeutralOpen(),
             bhStats().pctNeutralOpen(), true, true);

  format.subtitle(SLIPPAGE);
  format.row(AVERAGE_SLIPPAGE_PER_POS,
             totalStats().allPosStats().averageSlippagePerPos(),
             longStats().allPosStats().averageSlippagePerPos(),
             shortStats().allPosStats().averageSlippagePerPos(),
             bhStats().allPosStats().averageSlippagePerPos(), true);

  format.subtitle(COMMISSION);
  format.row(AVERAGE_COMMISSION_PER_POS,
             totalStats().allPosStats().averageCommissionPerPos(),
             longStats().allPosStats().averageCommissionPerPos(),
             shortStats().allPosStats().averageCommissionPerPos(),
             bhStats().allPosStats().averageCommissionPerPos(), true);
  format.footer();
}
