// Web page served at http://192.168.4.1
// Generated from web/index.html by tools/make_page.py - do not edit by hand.
#pragma once

const char INDEX_HTML[] = R"rawliteral(<!doctype html>
<html lang="de"><head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no,viewport-fit=cover">
<meta name="apple-mobile-web-app-capable" content="yes">
<meta name="mobile-web-app-capable" content="yes">
<meta name="apple-mobile-web-app-status-bar-style" content="black">
<meta name="theme-color" content="#000000">
<title>Wall Ball Counter</title>
<style>:root{--vh:1vh}

*{box-sizing:border-box;margin:0;padding:0}
html,body{height:100%;background:#000;color:#fff;overflow:hidden;font-family:-apple-system,"Helvetica Neue",Roboto,Arial,sans-serif;-webkit-user-select:none;user-select:none;touch-action:manipulation;-webkit-tap-highlight-color:transparent}
#app{position:fixed;left:0;top:0;right:0;height:calc(100*var(--vh))}
.num{font-weight:600;font-variant-numeric:tabular-nums}
.lbl{color:#B4B2A9}
#logo{position:absolute;top:calc(11.5*var(--vh));left:7.8vw;height:calc(14*var(--vh))}
#center{position:absolute;left:16vw;right:22vw;top:0;bottom:0;display:flex;flex-direction:column;align-items:center;justify-content:center}
#reps{font-size:min(calc(41*var(--vh)),21vw);line-height:.9;letter-spacing:-.02em;cursor:pointer}
#line{width:36vw;height:calc(1*var(--vh));background:#fff;margin:calc(3.5*var(--vh)) 0 calc(2.5*var(--vh))}
#goal{font-size:min(calc(15*var(--vh)),8vw);line-height:1;color:#B4B2A9;cursor:pointer;padding:0 3vw}
#side{position:absolute;right:4.4vw;top:calc(9*var(--vh));bottom:calc(6*var(--vh));width:15.6vw;display:flex;flex-direction:column;align-items:center;justify-content:space-between}
#target{height:calc(58*var(--vh));display:block;overflow:visible;margin-top:calc(7.25*var(--vh))}
.tap{fill:#888780;font-size:13px;font-weight:700;letter-spacing:3px;pointer-events:none}
.half{cursor:pointer}
.half .ol,.half .xl{fill:none;stroke:#444441;stroke-linecap:round;stroke-linejoin:round;transition:stroke .15s}
.half .ol{stroke-width:7}
.half .xl{stroke-width:3.6;stroke-linecap:butt}
.half .dot{fill:#444441;transition:fill .15s}
.half.on .ol,.half.on .xl{stroke:#97C459}
.half.on .dot{fill:#97C459}
.half .hit{fill:transparent}
#avgbox{text-align:center}
#avgbox .lbl{font-size:calc(3*var(--vh))}
#avg{font-size:calc(8*var(--vh));line-height:1}
#nr{position:absolute;left:7.8vw;bottom:calc(6*var(--vh));display:flex;align-items:center;gap:calc(2.8*var(--vh))}
#x{width:calc(9.5*var(--vh));height:calc(9.5*var(--vh));border-radius:50%;border:calc(.6*var(--vh)) solid #E24B4A;display:flex;align-items:center;justify-content:center}
#x svg{width:calc(5*var(--vh));height:calc(5*var(--vh))}
#norep{font-size:calc(8*var(--vh));line-height:1}
#nr .lbl{font-size:calc(3*var(--vh))}
#conn{position:absolute;top:calc(2.5*var(--vh));left:0;right:0;text-align:center;font-size:calc(2.4*var(--vh));color:#E24B4A;display:none}
#conn.show{display:block}
#flash{position:absolute;inset:0;border:1.4vmin solid transparent;pointer-events:none;transition:border-color .3s}
#flash.hit{border-color:#97C459;transition:none}
#flash.norep{border-color:#E24B4A;transition:none}
#go{position:absolute;inset:0;background:#639922;display:none;align-items:center;justify-content:center}
#go.show{display:flex}
#go span{font-size:min(calc(52*var(--vh)),30vw);font-weight:800;letter-spacing:-.02em}
.ov{position:absolute;inset:0;background:rgba(0,0,0,.75);display:none;align-items:center;justify-content:center}
.ov.show{display:flex}
.dlg{width:min(720px,92vw);background:#1c1c1b;border:2px solid #444441;border-radius:min(28px,4vmin);padding:min(44px,5vmin) min(48px,5vmin);text-align:center}
.dlg .t{font-size:min(34px,5vmin);color:#B4B2A9;margin-bottom:min(26px,3vmin)}
.row{display:flex;align-items:center;justify-content:center;gap:min(40px,4vmin)}
.step{width:min(120px,15vmin);height:min(120px,15vmin);border-radius:min(24px,3vmin);border:3px solid #5F5E5A;background:none;color:#fff;font-size:min(78px,10vmin);display:flex;align-items:center;justify-content:center;cursor:pointer}
#gval{font-size:min(170px,22vmin);min-width:min(300px,38vmin);line-height:1}
.presets{display:flex;gap:min(18px,2vmin);justify-content:center;margin:min(36px,4vmin) 0 min(34px,4vmin);flex-wrap:wrap}
.p{width:min(104px,13vmin);height:min(76px,10vmin);border-radius:min(18px,2.4vmin);background:#2C2C2A;border:none;color:#fff;font-size:min(36px,5vmin);font-weight:600;cursor:pointer}
.p.on{background:#97C459;color:#173404}
.btns{display:flex;gap:min(20px,2.5vmin)}
.b{flex:1;height:min(92px,12vmin);border-radius:min(20px,2.6vmin);font-size:min(36px,5vmin);font-weight:600;cursor:pointer}
.cancel{background:none;border:3px solid #5F5E5A;color:#D3D1C7}
.ok{background:#fff;border:none;color:#000}
.danger{background:#E24B4A;border:none;color:#fff}
.dlg .msg{font-size:min(40px,6vmin);margin-bottom:min(40px,5vmin)}
#gear{position:absolute;top:calc(3*var(--vh));right:2.5vw;width:min(calc(6*var(--vh)),6vw);height:min(calc(6*var(--vh)),6vw);cursor:pointer;opacity:.55}
#dlgPin .dlg{width:min(560px,70vw);padding:calc(4*var(--vh)) min(44px,4vw)}
#dlgPin .t{font-size:calc(3.6*var(--vh));margin-bottom:calc(2.5*var(--vh))}
.pdots{display:flex;gap:calc(2.6*var(--vh));justify-content:center}
.pdots i{width:calc(3*var(--vh));height:calc(3*var(--vh));border-radius:50%;border:3px solid #888780;display:block;transition:background .1s,border-color .1s}
.pdots i.on{background:#fff;border-color:#fff}
.pdots.err i{border-color:#E24B4A;background:#E24B4A}
.pdots.err{animation:shake .35s}
@keyframes shake{25%{transform:translateX(-12px)}50%{transform:translateX(12px)}75%{transform:translateX(-6px)}}
.pmsg{height:calc(3.4*var(--vh));line-height:calc(3.4*var(--vh));margin:calc(1.4*var(--vh)) 0 calc(1.6*var(--vh));font-size:calc(2.3*var(--vh));color:#888780}
.pmsg.err{color:#F09595}.pmsg.okc{color:#C0DD97}
.pkeys{display:grid;grid-template-columns:repeat(3,1fr);gap:calc(1.6*var(--vh))}
.pk{height:calc(9.5*var(--vh));border-radius:calc(2*var(--vh));background:#2C2C2A;border:none;color:#fff;font-size:calc(4.6*var(--vh));font-weight:600;cursor:pointer;display:flex;align-items:center;justify-content:center;font-variant-numeric:tabular-nums}
.pk:active{background:#444441}
.pk.txt{background:none;border:2px solid #5F5E5A;color:#D3D1C7;font-size:calc(2.6*var(--vh))}
.pk svg{width:calc(4.4*var(--vh));height:calc(4.4*var(--vh))}
#spPin,#spCal{height:calc(7*var(--vh));padding:0 1.4vw;border-radius:calc(1.75*var(--vh));border:2px solid #5F5E5A;background:none;color:#D3D1C7;font-size:calc(2.3*var(--vh));display:flex;align-items:center;gap:.7vw;cursor:pointer}
#spPin svg,#spCal svg{width:calc(2.8*var(--vh));height:calc(2.8*var(--vh))}
.sp-hdr .hbtns{display:flex;gap:1vw}
#spCal.run{border-color:#EF9F27;color:#FAC775}
#spCal.done{border-color:#97C459;color:#C0DD97}
@media (orientation:portrait) and (max-width:600px){.sp-hdr{flex-wrap:wrap;row-gap:calc(2*var(--vh))}.sp-hdr .hbtns{width:100%;gap:2.5vw}#spPin,#spCal{flex:1;justify-content:center;white-space:nowrap}}
/* settings page (tablet, landscape): frozen impact graph left, settings right */
#setPage{position:absolute;inset:0;background:#000;display:none}
#setPage.show{display:block}
.sp-hdr{position:absolute;top:calc(3.25*var(--vh));left:3.4vw;right:3.4vw;display:flex;align-items:center;justify-content:space-between}
.sp-title{font-size:calc(4.25*var(--vh));font-weight:600}
#spClose{width:calc(7*var(--vh));height:calc(7*var(--vh));border-radius:calc(1.75*var(--vh));border:2px solid #5F5E5A;background:none;display:flex;align-items:center;justify-content:center;cursor:pointer}
#spClose svg{width:calc(3.25*var(--vh));height:calc(3.25*var(--vh))}
#plotBox{position:absolute;top:calc(12.5*var(--vh));left:3.4vw;width:46.9vw;bottom:calc(3.25*var(--vh));background:#111110;border-radius:calc(2*var(--vh));overflow:hidden}
#plot{position:absolute;inset:0;width:100%;height:100%}
.ptop{position:absolute;top:calc(2*var(--vh));left:1.6vw;right:16vw}
.ptitle{font-size:calc(2.5*var(--vh));font-weight:600;white-space:nowrap}
.psub{font-size:calc(2*var(--vh));color:#888780;margin-top:calc(.5*var(--vh));white-space:nowrap;font-variant-numeric:tabular-nums}
.pill{position:absolute;top:calc(1.75*var(--vh));right:1.4vw;font-size:calc(1.75*var(--vh));font-weight:600;border-radius:calc(1*var(--vh));padding:calc(.75*var(--vh)) .8vw;display:flex;align-items:center;gap:.55vw;color:#C0DD97;background:#173404;white-space:nowrap}
.pill i{width:calc(1.1*var(--vh));height:calc(1.1*var(--vh));border-radius:50%;background:#97C459;display:inline-block}
.pill.rec{color:#FAC775;background:#412402}.pill.rec i{background:#EF9F27}
.pill.old{color:#D3D1C7;background:#2C2C2A}.pill.old i{background:#888780}
.seg{position:absolute;top:calc(10.25*var(--vh));left:1.6vw;display:flex;align-items:center;gap:.5vw;font-size:calc(1.8*var(--vh));color:#888780}
.seg button{min-width:calc(6*var(--vh));height:calc(4.25*var(--vh));border-radius:calc(1*var(--vh));border:1.5px solid #444441;background:none;color:#D3D1C7;font-size:calc(1.9*var(--vh));font-weight:600;cursor:pointer;font-variant-numeric:tabular-nums}
.seg button.on{background:#97C459;border-color:#97C459;color:#173404}
.legend{position:absolute;bottom:calc(1.75*var(--vh));left:1.6vw;right:1.6vw;display:flex;flex-wrap:wrap;gap:calc(1*var(--vh)) 1.4vw;font-size:calc(1.9*var(--vh));color:#D3D1C7}
.legend i{display:inline-block;width:calc(2.5*var(--vh));height:calc(.5*var(--vh));border-radius:calc(.25*var(--vh));vertical-align:middle;margin-right:.55vw}
.legend b{display:inline-block;width:calc(1.75*var(--vh));height:calc(1.75*var(--vh));border-radius:calc(.4*var(--vh));vertical-align:middle;margin-right:.55vw;background:rgba(151,196,89,.22);border:1px solid rgba(151,196,89,.5)}
.sp-right{position:absolute;top:calc(12.5*var(--vh));left:52.2vw;right:3.4vw;bottom:calc(3.25*var(--vh));display:flex;flex-direction:column;gap:calc(2*var(--vh))}
.box{background:#111110;border-radius:calc(2*var(--vh));padding:calc(1.75*var(--vh)) 1.6vw}
.ctrls{display:flex;flex-direction:column;gap:calc(1.25*var(--vh))}
.srow{display:flex;align-items:center;justify-content:space-between;gap:1vw}
.srow .name{font-size:calc(2.6*var(--vh));white-space:nowrap}
.srow .name small{display:block;font-size:calc(1.6*var(--vh));color:#888780;margin-top:calc(.25*var(--vh))}
.sctl{display:flex;align-items:center;gap:.8vw}
.sctl .step{width:calc(6.25*var(--vh));height:calc(6.25*var(--vh));border-radius:calc(1.5*var(--vh));font-size:calc(3.75*var(--vh));border-width:2px}
.sval{white-space:nowrap;font-size:calc(3.5*var(--vh));min-width:calc(14*var(--vh));text-align:center;font-weight:600;font-variant-numeric:tabular-nums}
.hits{flex:1;display:flex;flex-direction:column;min-width:0;min-height:0;overflow:hidden}
.hits h3,.buf h3{font-size:calc(2*var(--vh));font-weight:500;color:#B4B2A9;margin-bottom:calc(.5*var(--vh))}
.hits h3 small{font-size:calc(1.6*var(--vh));color:#5F5E5A;font-weight:400;margin-left:.6vw}
#hitList{flex:1;min-height:0;overflow:hidden;display:flex;flex-direction:column}
.hrow{display:grid;grid-template-columns:calc(7.75*var(--vh)) 1fr calc(11.5*var(--vh)) calc(11.5*var(--vh));align-items:center;font-size:calc(2.4*var(--vh));flex:0 1 max(46px,calc(5.6*var(--vh)));min-height:calc(3.8*var(--vh));padding:0 .8vw;font-variant-numeric:tabular-nums;white-space:nowrap;cursor:pointer;border-radius:calc(1*var(--vh))}
.hrow:active{background:#2C2C2A}
.hrow+.hrow{border-top:1px solid #2C2C2A}
.hrow.sel{background:#1f1f1e}
.hrow.wait{opacity:.55;cursor:default}
.ago{color:#888780;font-size:calc(1.9*var(--vh))}
.dot{display:inline-block;width:calc(1.4*var(--vh));height:calc(1.4*var(--vh));border-radius:50%;margin-right:.55vw;vertical-align:middle}
.g{font-weight:600;text-align:right;padding-right:1vw}
.tag{font-size:calc(1.6*var(--vh));font-weight:600;padding:calc(.5*var(--vh)) .55vw;border-radius:calc(.9*var(--vh));text-align:center}
.rep{background:#27500A;color:#C0DD97}
.no{background:#501313;color:#F7C1C1}
.empty{color:#5F5E5A;font-size:calc(2.1*var(--vh));padding:calc(.5*var(--vh)) 0}
.buf{margin-top:auto;padding-top:calc(.9*var(--vh));border-top:1px solid #2C2C2A;display:flex;align-items:center;justify-content:space-between}
.buf h3{margin:0}
#bufVal{font-size:calc(2*var(--vh));font-weight:600;color:#888780;display:flex;align-items:center;gap:.5vw}
#bufVal.warn{color:#F09595}
#bufVal svg{width:calc(2.25*var(--vh));height:calc(2.25*var(--vh))}
.sp-btns{display:flex;gap:1.1vw;height:calc(9.25*var(--vh))}
.sp-btns .b{flex:1;height:auto;border-radius:calc(2*var(--vh));font-size:calc(3*var(--vh))}

@media (orientation:portrait){
  #center{left:0;right:0;top:calc(6*var(--vh));bottom:auto;justify-content:flex-start}
  #reps{font-size:min(calc(25*var(--vh)),54vw)}
  #line{width:56vw;height:calc(.6*var(--vh));margin:calc(2.1*var(--vh)) 0 calc(1.4*var(--vh))}
  #goal{font-size:calc(8.5*var(--vh))}
  #side{left:0;right:0;top:calc(49*var(--vh));bottom:auto;width:auto;display:block}
  #target{height:calc(27*var(--vh));margin:0 8vw 0 auto}
  #avgbox{position:absolute;left:8vw;top:calc(4*var(--vh));text-align:left}
  #avgbox .lbl{font-size:calc(2.1*var(--vh))}
  #avg{font-size:calc(5.2*var(--vh))}
  #nr{left:8vw;top:calc(65*var(--vh));bottom:auto;gap:calc(1.7*var(--vh))}
  #x{width:calc(6.2*var(--vh));height:calc(6.2*var(--vh));border-width:calc(.45*var(--vh))}
  #x svg{width:calc(3.3*var(--vh));height:calc(3.3*var(--vh))}
  #norep{font-size:calc(5*var(--vh))}
  #nr .lbl{font-size:calc(1.9*var(--vh))}
  #logo{top:auto;bottom:calc(4.5*var(--vh));left:8vw;height:calc(8.3*var(--vh))}
  #gear{top:auto;bottom:calc(5.9*var(--vh));right:8vw;width:calc(5.5*var(--vh));height:calc(5.5*var(--vh))}
  #go span{font-size:min(calc(20*var(--vh)),40vw)}
}

@media (orientation:portrait){
#setPage{--vh:clamp(5.4px,1vw,10px)}
#setPage.show{display:flex;flex-direction:column;gap:calc(2*var(--vh));padding:calc(3*var(--vh)) 4vw calc(3*var(--vh));overflow-y:auto}
.sp-hdr{position:static;flex:none}
#plotBox{position:relative;top:auto;left:auto;bottom:auto;width:auto;flex:1 1 42%;min-height:calc(58*var(--vh))}
.ptop{right:1.6vw;top:calc(6.2*var(--vh))}
.pill{left:1.6vw;right:auto;top:calc(1.9*var(--vh))}
.seg{top:calc(13.6*var(--vh))}
.sp-right{position:static;flex:none;display:flex;flex-direction:column;gap:calc(2*var(--vh))}
.ctrls{display:grid;grid-template-columns:1fr 1fr;gap:calc(2.2*var(--vh)) 5vw}
.srow{flex-direction:column;align-items:stretch;gap:calc(1*var(--vh))}
.sctl{justify-content:space-between}
.sval{min-width:0;flex:1}
.hits{flex:none}
.sp-btns{flex:none}
.sp-btns .b{height:calc(9*var(--vh));font-size:calc(3.4*var(--vh))}
}
</style>
</head>
<body>
<script>
(function(){function setVh(){document.documentElement.style.setProperty('--vh',(window.innerHeight/100)+'px');}
setVh();window.addEventListener('resize',setVh);window.addEventListener('orientationchange',function(){setTimeout(setVh,300);});})();
</script>
<div id="app">
  <img id="logo" src="/logo.png" alt="Logo">
  <div id="center">
    <div id="reps" class="num">0</div>
    <div id="line"></div>
    <div id="goal" class="num">20</div>
  </div>
  <div id="side">
    <svg id="target" viewBox="-5 -5 110 220" aria-label="Aktive Zone wählen">
      <g id="up" class="half on">
        <rect class="hit" x="-5" y="-5" width="110" height="110"/>
        <path class="ol" d="M13,105.3 A13.2,13.2 0 0 0 0,92.1 L8.5,7 Q9,0 15,0 L85,0 Q91,0 91.5,7 L100,92.1 A13.2,13.2 0 0 0 87,105.3"/>
        <path class="xl" d="M29,32 L44,47 M71,32 L56,47 M29,76 L44,61 M71,76 L56,61"/>
        <circle class="dot" cx="50" cy="54" r="3.8"/>
      </g>
      <g id="down" class="half">
        <rect class="hit" x="-5" y="105" width="110" height="110"/>
        <path class="ol" d="M13,105.3 A13.2,13.2 0 0 1 0,118.5 L8.5,203 Q9,210 15,210 L85,210 Q91,210 91.5,203 L100,118.5 A13.2,13.2 0 0 1 87,105.3"/>
        <path class="xl" d="M29,144 L44,159 M71,144 L56,159 M29,188 L44,173 M71,188 L56,173"/>
        <circle class="dot" cx="50" cy="166" r="3.8"/>
      </g>
      <text class="tap" x="51.5" y="105.3" text-anchor="middle" dominant-baseline="central">TAP</text>
    </svg>
    <div id="avgbox"><div id="avg" class="num">– s</div><div class="lbl">Rep Avg</div></div>
  </div>
  <div id="nr">
    <div id="x"><svg viewBox="0 0 24 24"><path d="M6 6L18 18M18 6L6 18" stroke="#E24B4A" stroke-width="3" stroke-linecap="round"/></svg></div>
    <div><div id="norep" class="num">0</div><div class="lbl">No Reps</div></div>
  </div>
  <svg id="gear" viewBox="0 0 24 24" fill="none" stroke="#B4B2A9" stroke-width="1.6" stroke-linecap="round" stroke-linejoin="round" aria-label="Einstellungen"><path d="M10.325 4.317c.426-1.756 2.924-1.756 3.35 0a1.724 1.724 0 0 0 2.573 1.066c1.543-.94 3.31.826 2.37 2.37a1.724 1.724 0 0 0 1.065 2.572c1.756.426 1.756 2.924 0 3.35a1.724 1.724 0 0 0-1.066 2.573c.94 1.543-.826 3.31-2.37 2.37a1.724 1.724 0 0 0-2.572 1.065c-.426 1.756-2.924 1.756-3.35 0a1.724 1.724 0 0 0-2.573-1.066c-1.543.94-3.31-.826-2.37-2.37a1.724 1.724 0 0 0-1.065-2.572c-1.756-.426-1.756-2.924 0-3.35a1.724 1.724 0 0 0 1.066-2.573c-.94-1.543.826-3.31 2.37-2.37c1 .608 2.296.07 2.572-1.065z"/><circle cx="12" cy="12" r="3"/></svg>
  <div id="conn">Keine Verbindung zum Target</div>
  <div id="flash"></div>
  <div id="go"><span>GO!</span></div>

  <div id="dlgGoal" class="ov">
    <div class="dlg">
      <div class="t">Zielanzahl</div>
      <div class="row">
        <button class="step" id="minus">&minus;</button>
        <div id="gval" class="num">100</div>
        <button class="step" id="plus">+</button>
      </div>
      <div class="presets" id="presets"></div>
      <div class="btns">
        <button class="b cancel" id="gCancel">Abbrechen</button>
        <button class="b ok" id="gOk">Übernehmen</button>
      </div>
    </div>
  </div>

  <div id="setPage">
    <div class="sp-hdr">
      <div class="sp-title">Einstellungen</div>
      <div class="hbtns"><button id="spCal"><svg viewBox="0 0 24 24" fill="none" stroke="#D3D1C7" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="12" r="8"/><circle cx="12" cy="12" r="2"/><path d="M12 2v3M12 19v3M2 12h3M19 12h3"/></svg><span id="spCalTxt">Kalibrieren</span></button><button id="spPin"><svg viewBox="0 0 24 24" fill="none" stroke="#D3D1C7" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><rect x="5" y="11" width="14" height="10" rx="2"/><path d="M8 11V7a4 4 0 0 1 8 0v4"/></svg>Code ändern</button>
      <button id="spClose" aria-label="Schließen"><svg viewBox="0 0 24 24"><path d="M5 5L19 19M19 5L5 19" stroke="#D3D1C7" stroke-width="2.5" stroke-linecap="round"/></svg></button></div>
    </div>
    <div id="plotBox">
      <svg id="plot"></svg>
      <div class="ptop">
        <div class="ptitle" id="pTitle">Noch kein Aufprall</div>
        <div class="psub" id="pSub">Wirf einen Ball – die Aufnahme erscheint hier</div>
      </div>
      <div class="seg" id="seg"><span>Anzeige nach Aufprall</span><button data-v="0.2">0.2</button><button data-v="0.4">0.4</button><button data-v="0.6">0.6</button><button data-v="0.8">0.8</button><span>s</span></div>
      <div class="pill" id="pill"><i></i><span id="pillTxt">Wartet auf Aufprall</span></div>
      <div class="legend">
        <span><i style="background:#378ADD"></i>↑ Sensor O</span>
        <span><i style="background:#EF9F27"></i>↓ Sensor U</span>
        <span><i style="background:#97C459"></i>Treffer ab</span>
        <span><i style="background:#888780"></i>Rauschen</span>
        <span><b></b>Messfenster</span>
      </div>
    </div>
    <div class="sp-right">
      <div class="box ctrls">
        <div class="srow"><div class="name">Rauschen bis<small>darunter ignoriert</small></div>
          <div class="sctl"><button class="step" id="nMinus">&minus;</button><div class="sval" id="nVal">0.10 g</div><button class="step" id="nPlus">+</button></div></div>
        <div class="srow"><div class="name">Treffer ab<small>darunter No Rep</small></div>
          <div class="sctl"><button class="step" id="hMinus">&minus;</button><div class="sval" id="hVal">3.0 g</div><button class="step" id="hPlus">+</button></div></div>
        <div class="srow"><div class="name">Sperrzeit<small>Abstand zwischen Treffern</small></div>
          <div class="sctl"><button class="step" id="lMinus">&minus;</button><div class="sval" id="lVal">1.0 s</div><button class="step" id="lPlus">+</button></div></div>
        <div class="srow"><div class="name">Messfenster<small>Spitzenwert-Suche nach Aufprall</small></div>
          <div class="sctl"><button class="step" id="wMinus">&minus;</button><div class="sval" id="wVal">100 ms</div><button class="step" id="wPlus">+</button></div></div>
      </div>
      <div class="box hits">
        <h3>Letzte 5 Aufpralle<small>antippen zum Anzeigen</small></h3>
        <div id="hitList"><div class="empty">noch keine</div></div>
        <div class="buf"><h3>Puffer</h3><span id="bufVal">OK</span></div>
      </div>
      <div class="sp-btns">
        <button class="b cancel" id="sCancel">Abbrechen</button>
        <button class="b ok" id="sOk">Speichern</button>
      </div>
    </div>
  </div>

  <div id="dlgPin" class="ov">
    <div class="dlg">
      <div class="t" id="pinTitle">Code eingeben</div>
      <div class="pdots" id="pinDots"><i></i><i></i><i></i><i></i></div>
      <div class="pmsg" id="pinMsg">Einstellungen sind geschützt</div>
      <div class="pkeys" id="pinKeys">
        <button class="pk" data-k="1">1</button><button class="pk" data-k="2">2</button><button class="pk" data-k="3">3</button>
        <button class="pk" data-k="4">4</button><button class="pk" data-k="5">5</button><button class="pk" data-k="6">6</button>
        <button class="pk" data-k="7">7</button><button class="pk" data-k="8">8</button><button class="pk" data-k="9">9</button>
        <button class="pk txt" id="pinCancel">Abbrechen</button><button class="pk" data-k="0">0</button>
        <button class="pk txt" id="pinDel" aria-label="Löschen"><svg viewBox="0 0 24 24" fill="none" stroke="#D3D1C7" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M20 6H9l-5 6 5 6h11a1 1 0 0 0 1-1V7a1 1 0 0 0-1-1z"/><path d="M12 10l4 4M16 10l-4 4"/></svg></button>
      </div>
    </div>
  </div>

  <div id="dlgReset" class="ov">
    <div class="dlg">
      <div class="msg">Zähler zurücksetzen?</div>
      <div class="btns">
        <button class="b cancel" id="rCancel">Abbrechen</button>
        <button class="b danger" id="rOk">Zurücksetzen</button>
      </div>
    </div>
  </div>
</div>

<script>
const $ = id => document.getElementById(id);
const PRESETS = [20, 40, 60, 80, 100];
let st = null, prevDone = false, prevSeq = null, goTimer = null, flashTimer = null;
let pendingGoal = 20, busy = false;

function fmtAvg(ms) { return ms > 0 ? (ms / 1000).toFixed(2) + ' s' : '– s'; }

function setZoneUI(z) {
  $('up').classList.toggle('on', z === 'up');
  $('down').classList.toggle('on', z === 'down');
}

function flash(kind) {
  const f = $('flash');
  f.className = kind;
  clearTimeout(flashTimer);
  flashTimer = setTimeout(() => { f.className = ''; }, 350);
}

function showGo() {
  $('go').classList.add('show');
  clearTimeout(goTimer);
  goTimer = setTimeout(() => $('go').classList.remove('show'), 5000);
}

function render(s) {
  $('reps').textContent = s.reps;
  $('goal').textContent = s.goal;
  $('norep').textContent = s.noReps;
  $('avg').textContent = fmtAvg(s.avgMs);
  setZoneUI(s.zone);
  if (prevSeq !== null && s.seq !== prevSeq) {
    if (s.last === 'hit') flash('hit');
    else if (s.last === 'weak' || s.last === 'wrong') flash('norep');
  }
  if (st && !prevDone && s.done) showGo();
  prevSeq = s.seq; prevDone = s.done; st = s;
  updateWarn();
}

let connOk = true;
function setConn(ok) { connOk = ok; updateWarn(); }
function updateWarn() {
  let msg = '';
  if (!connOk) msg = 'Keine Verbindung zum Target';
  else if (st && st.zone === 'up' && !(st.sensors & 1)) msg = 'Sensor O (oben) nicht gefunden';
  else if (st && st.zone === 'down' && !(st.sensors & 2)) msg = 'Sensor U (unten) nicht gefunden';
  $('conn').textContent = msg;
  $('conn').classList.toggle('show', msg !== '');
}

async function get(url) {
  const c = new AbortController();
  const t = setTimeout(() => c.abort(), 2500);
  try {
    const r = await fetch(url, { cache: 'no-store', signal: c.signal });
    return await r.json();
  } finally { clearTimeout(t); }
}

async function poll() {
  if (!busy && !plotOpen) {          // the settings page polls /plot instead
    try { render(await get('/state')); setConn(true); }
    catch (e) { setConn(false); }
  }
  setTimeout(poll, 300);
}

async function cmd(url) {
  busy = true;
  try { render(await get(url)); setConn(true); }
  catch (e) { setConn(false); }
  busy = false;
}

// Zone arrows
$('up').onclick = () => { setZoneUI('up'); cmd('/zone?z=up'); };
$('down').onclick = () => { setZoneUI('down'); cmd('/zone?z=down'); };

// Goal dialog
function renderGoalDlg() {
  $('gval').textContent = pendingGoal;
  document.querySelectorAll('.p').forEach(b => b.classList.toggle('on', +b.dataset.v === pendingGoal));
}
PRESETS.forEach(v => {
  const b = document.createElement('button');
  b.className = 'p'; b.textContent = v; b.dataset.v = v;
  b.onclick = () => { pendingGoal = v; renderGoalDlg(); };
  $('presets').appendChild(b);
});
$('goal').onclick = () => { pendingGoal = st ? st.goal : 20; renderGoalDlg(); $('dlgGoal').classList.add('show'); };
$('gCancel').onclick = () => $('dlgGoal').classList.remove('show');
$('gOk').onclick = () => { $('dlgGoal').classList.remove('show'); cmd('/goal?n=' + pendingGoal); };

function holdRepeat(el, step) {
  let t1 = null, t2 = null;
  const stop = () => { clearTimeout(t1); clearInterval(t2); };
  el.addEventListener('pointerdown', e => {
    e.preventDefault(); step();
    t1 = setTimeout(() => { t2 = setInterval(step, 70); }, 400);
  });
  ['pointerup', 'pointerleave', 'pointercancel'].forEach(ev => el.addEventListener(ev, stop));
}
const goalStep = d => () => { pendingGoal = Math.min(999, Math.max(1, pendingGoal + d)); renderGoalDlg(); };
holdRepeat($('minus'), goalStep(-1));
holdRepeat($('plus'), goalStep(1));

// ---------- Settings page (gear icon) – values in 1/100 g and ms ----------
// The graph shows a frozen 2 s recording of one impact (0.5 s before, 1.5 s after).
// It switches to the newest impact automatically; tapping a row shows an older one
// until the next impact comes in.
let pHit = 300, pNoise = 10, pLock = 1000, pWin = 100;
let plotOpen = false, plotTimer = null, dirty = true;
let hits = [], ovf = [0, 0];
let snapData = null, shownId = 0, pinned = false, newestSeen = 0, fetching = false;
let yCur = 10, yTarget = 10;
// visible time after the impact (0.2–0.8 s), remembered on this device
let viewPost = 0.8;
const VIEW_PRE = 0.2;                   // seconds shown before the impact
try { const v = parseFloat(localStorage.getItem('lux.viewPost')); if ([0.2, 0.4, 0.6, 0.8].includes(v)) viewPost = v; } catch (e) {}

// Scale: the next whole g above the highest value in the visible part, so the
// whole curve always fills the graph. The hit threshold counts too, so its line
// never disappears off the top.
function pickScale() {
  let m = pHit / 100;
  if (snapData) {
    const first = Math.max(0, Math.floor((snapData.pre - VIEW_PRE * 1000) / snapData.step));
    const last = Math.floor((snapData.pre + viewPost * 1000) / snapData.step);
    for (let i = first; i < last && i < snapData.a.length; i++) m = Math.max(m, snapData.a[i] / 100, snapData.b[i] / 100);
  }
  // 5 % headroom, so a peak never sits exactly on the top edge (2.99 g -> 4 g, 2.5 g -> 3 g)
  yTarget = Math.max(1, Math.min(16, Math.ceil(m * 1.05 - 0.001)));
  dirty = true;
}
function renderSeg() {
  document.querySelectorAll('#seg button').forEach(b => b.classList.toggle('on', +b.dataset.v === viewPost));
}
$('seg').addEventListener('click', e => {
  const b = e.target.closest('button');
  if (!b) return;
  viewPost = +b.dataset.v;
  try { localStorage.setItem('lux.viewPost', String(viewPost)); } catch (e) {}
  renderSeg(); pickScale();
});
const COL = { 1: '#378ADD', 2: '#EF9F27' };
// hit row: [ms ago, sensor, peak 1/100 g, rep?, id, ready?, ms to peak, window ms]
const H_AGO = 0, H_SENS = 1, H_PEAK = 2, H_REP = 3, H_ID = 4, H_READY = 5, H_OFF = 6, H_WIN = 7;

function renderSetDlg() {
  $('nVal').textContent = (pNoise / 100).toFixed(2) + ' g';
  $('hVal').textContent = (pHit / 100).toFixed(1) + ' g';
  $('lVal').textContent = (pLock / 1000).toFixed(1) + ' s';
  $('wVal').textContent = pWin + ' ms';
  pickScale();                          // also redraws: lines and window band follow live                         // lines and window band follow the steppers live
}

function fmtAgo(ms) {
  const s = ms / 1000;
  return s < 60 ? '−' + s.toFixed(1) + ' s' : '−' + Math.floor(s / 60) + ' min';
}
function hitById(id) { return hits.find(h => h[H_ID] === id); }

function renderHits() {
  if (!hits.length) { $('hitList').innerHTML = '<div class="empty">noch keine</div>'; return; }
  $('hitList').innerHTML = hits.map(h => {
    const cls = 'hrow' + (h[H_ID] === shownId ? ' sel' : '') + (h[H_READY] ? '' : ' wait');
    return '<div class="' + cls + '" data-id="' + h[H_ID] + '"><span class="ago">' + fmtAgo(h[H_AGO]) +
      '</span><span><span class="dot" style="background:' + COL[h[H_SENS]] + '"></span>' +
      (h[H_SENS] === 1 ? '↑ Sensor O' : '↓ Sensor U') + '</span><span class="g">' + (h[H_PEAK] / 100).toFixed(2) +
      ' g</span><span class="tag ' + (h[H_REP] ? 'rep' : 'no') + '">' + (h[H_REP] ? 'Rep' : 'No Rep') + '</span></div>';
  }).join('');
}
$('hitList').addEventListener('click', e => {
  const row = e.target.closest('.hrow');
  if (!row) return;
  const id = +row.dataset.id, h = hitById(id);
  if (!h || !h[H_READY]) return;          // recording still running
  pinned = hits.length > 0 && id !== hits[0][H_ID];
  loadSnap(id);
});

const WARN_ICON = '<svg viewBox="0 0 24 24"><path d="M12 3L22 20H2Z" fill="none" stroke="#F09595" stroke-width="2.2" stroke-linejoin="round"/><path d="M12 10v4M12 17v.5" stroke="#F09595" stroke-width="2.2" stroke-linecap="round"/></svg>';
function renderBuf() {
  const parts = [];
  if (ovf[0]) parts.push('Sensor O · ' + ovf[0] + '×');
  if (ovf[1]) parts.push('Sensor U · ' + ovf[1] + '×');
  const el = $('bufVal');
  el.classList.toggle('warn', parts.length > 0);
  el.innerHTML = parts.length ? WARN_ICON + 'Überlauf ' + parts.join(', ') + ' in 10 s' : 'OK';
}

function renderHeader() {
  const newest = hits[0];
  const pill = $('pill');
  let cls = '', txt = 'Wartet auf nächsten Aufprall';
  if (newest && !newest[H_READY]) { cls = 'rec'; txt = 'Aufprall erkannt – Aufnahme läuft'; }
  else if (pinned) { cls = 'old'; txt = 'Ältere Aufnahme'; }
  pill.className = 'pill' + (cls ? ' ' + cls : '');
  $('pillTxt').textContent = txt;

  const h = hitById(shownId);
  if (!snapData || !h) {
    $('pTitle').textContent = hits.length ? 'Aufnahme wird geladen' : 'Noch kein Aufprall';
    $('pSub').textContent = hits.length ? '' : 'Wirf einen Ball – die Aufnahme erscheint hier';
    return;
  }
  const s = h[H_SENS] === 1 ? '↑ Sensor O' : '↓ Sensor U';
  $('pTitle').textContent = (h === newest ? 'Letzter Aufprall · ' : 'Aufprall · ') + s;
  $('pSub').textContent = 'vor ' + (h[H_AGO] / 1000).toFixed(1) + ' s · Spitze ' + (h[H_PEAK] / 100).toFixed(2) +
    ' g · ' + (h[H_REP] ? 'Rep' : 'No Rep') + ' · Fenster ' + h[H_WIN] + ' ms';
}

async function loadSnap(id) {
  if (fetching) return;
  fetching = true;
  try {
    const d = await get('/snap?id=' + id);
    if (d.ready) {
      snapData = d; shownId = id;
      pickScale();
      renderHits(); renderHeader();
    }
  } catch (e) { setConn(false); }
  fetching = false;
}

async function plotPoll() {
  if (!plotOpen) return;
  try {
    const d = await get('/plot');
    setConn(true);
    hits = d.hits; ovf = d.ovf;
    const newest = hits[0];
    if (newest && newest[H_ID] !== newestSeen) {   // a new impact unfreezes the graph
      newestSeen = newest[H_ID];
      pinned = false;
    }
    if (!hits.length) { snapData = null; shownId = 0; dirty = true; }
    const want = pinned ? shownId : (newest ? newest[H_ID] : 0);
    const wh = hitById(want);
    if (want && want !== shownId && wh && wh[H_READY]) loadSnap(want);
    renderHits(); renderBuf(); renderHeader();
  } catch (e) { setConn(false); }
  if (plotOpen) plotTimer = setTimeout(plotPoll, 300);
}

function smoothPath(pts, base, top) {
  if (pts.length < 2) return '';
  const cl = y => Math.max(top, Math.min(base, y));        // keep the curve inside the plot
  let d = 'M' + pts[0][0].toFixed(1) + ',' + pts[0][1].toFixed(1);
  for (let i = 0; i < pts.length - 1; i++) {
    const p0 = pts[i - 1] || pts[i], p1 = pts[i], p2 = pts[i + 1], p3 = pts[i + 2] || p2;
    // control points stay between the two neighbouring values: smooth, but the
    // curve never rises above (or dips below) what was actually measured
    const lo = Math.min(p1[1], p2[1]), hi = Math.max(p1[1], p2[1]);
    const mono = y => Math.max(lo, Math.min(hi, y));
    const c1x = p1[0] + (p2[0] - p0[0]) / 6, c1y = cl(mono(p1[1] + (p2[1] - p0[1]) / 6));
    const c2x = p2[0] - (p3[0] - p1[0]) / 6, c2y = cl(mono(p2[1] - (p3[1] - p1[1]) / 6));
    d += 'C' + c1x.toFixed(1) + ',' + c1y.toFixed(1) + ' ' + c2x.toFixed(1) + ',' + c2y.toFixed(1) + ' ' +
         p2[0].toFixed(1) + ',' + p2[1].toFixed(1);
  }
  return d;
}

function drawPlot() {
  const svg = $('plot');
  const W = svg.clientWidth, H = svg.clientHeight;
  if (!W || !H) return;
  const sg = $('seg'), L = W * 0.103, R = W * 0.04, B = H * 0.165;
  const T = Math.max(H * 0.2, sg.offsetTop + sg.offsetHeight + H * 0.05);
  const pw = W - L - R, ph = H - T - B, base = T + ph;
  const fs = (H * 0.024).toFixed(1);
  const t0 = -VIEW_PRE, t1 = viewPost;
  const X = t => L + (t - t0) / (t1 - t0) * pw;
  const Y = g => base - Math.min(g, yCur) / yCur * ph;
  let s = '';

  // measuring window (live preview of the stepper value) and impact mark
  s += '<rect x="' + X(0).toFixed(1) + '" y="' + T.toFixed(1) + '" width="' + (X(Math.min(pWin / 1000, t1)) - X(0)).toFixed(1) +
       '" height="' + ph.toFixed(1) + '" fill="rgba(151,196,89,.16)"/>';
  s += '<line x1="' + X(0).toFixed(1) + '" x2="' + X(0).toFixed(1) + '" y1="' + T + '" y2="' + base +
       '" stroke="#5F5E5A" stroke-width="1.5" stroke-dasharray="3 4"/>';

  // about 4–8 grid lines whatever the scale; the top value always gets a line
  const step = yTarget <= 6 ? 1 : yTarget <= 12 ? 2 : 4;
  const gridVals = [];
  for (let g = 0; g <= yTarget + 0.001; g += step) gridVals.push(g);
  if (gridVals[gridVals.length - 1] !== yTarget) {
    // top value not on the grid: drop the line just below it if the two would crowd
    if (yTarget - gridVals[gridVals.length - 1] < step * 0.5 + 0.001 && gridVals.length > 1) gridVals.pop();
    gridVals.push(yTarget);
  }
  for (const g of gridVals) {
    if (g > yCur + 0.01) continue;
    const y = Y(g).toFixed(1);
    s += '<line x1="' + L + '" x2="' + (L + pw) + '" y1="' + y + '" y2="' + y + '" stroke="#2C2C2A" stroke-width="1"/>';
    s += '<text x="' + (L - 10) + '" y="' + (+y + 6) + '" fill="#888780" font-size="' + fs + '" text-anchor="end">' + g + ' g</text>';
  }
  // time axis: baseline, fine ticks every 0.1 s, longer ticks + labels every 0.2 s
  s += '<line x1="' + L + '" x2="' + (L + pw) + '" y1="' + base + '" y2="' + base + '" stroke="#5F5E5A" stroke-width="1"/>';
  const tickH = H * 0.012;
  for (let k = Math.round(t0 * 10); k <= Math.round(t1 * 10); k++) {
    const t = k / 10, major = k % 2 === 0;
    const x = X(t).toFixed(1);
    s += '<line x1="' + x + '" x2="' + x + '" y1="' + base + '" y2="' + (base + (major ? tickH * 1.8 : tickH)).toFixed(1) +
         '" stroke="' + (major ? '#B4B2A9' : '#5F5E5A') + '" stroke-width="' + (major ? 1.5 : 1) + '"/>';
    const isStart = k === Math.round(t0 * 10);
    const room = X(0) - L > W * 0.12;    // start label only if it doesn't touch "Aufprall"
    const label = isStart ? (room ? t.toFixed(1) + ' s' : '') : major ? (k === 0 ? 'Aufprall' : '+' + t.toFixed(1) + ' s') : '';
    if (label) s += '<text x="' + x + '" y="' + (base + H * 0.052).toFixed(1) + '" fill="' + (k === 0 ? '#D3D1C7' : '#888780') +
         '" font-size="' + fs + '" text-anchor="' + (isStart ? 'start' : 'middle') + '">' + label + '</text>';
  }
  const line = (g, col, dash, w) => {
    if (g > yCur) return '';
    const y = Y(g).toFixed(1);
    return '<line x1="' + L + '" x2="' + (L + pw) + '" y1="' + y + '" y2="' + y + '" stroke="' + col +
           '" stroke-width="' + w + '" stroke-dasharray="' + dash + '"/>';
  };
  s += line(pHit / 100, '#97C459', '8 6', 2);
  s += line(pNoise / 100, '#888780', '4 5', 1.5);

  const h = hitById(shownId);
  if (snapData && h) {
    const stepS = snapData.step / 1000, preS = snapData.pre / 1000;
    const curve = (arr, col, w) => {
      const pts = [];
      for (let i = 0; i < arr.length; i++) {
        const t = i * stepS - preS + stepS / 2;
        if (t < t0 - stepS) continue;       // only the visible time range
        if (t > t1 + stepS) break;
        pts.push([X(Math.max(t0, Math.min(t, t1))), Y(arr[i] / 100)]);
      }
      return '<path d="' + smoothPath(pts, base, T - 4) + '" fill="none" stroke="' + col + '" stroke-width="' + w +
             '" stroke-linejoin="round"/>';
    };
    const active = h[H_SENS];             // the sensor that counted is drawn on top, thicker
    s += active === 1 ? curve(snapData.b, COL[2], 2) + curve(snapData.a, COL[1], 3)
                      : curve(snapData.a, COL[1], 2) + curve(snapData.b, COL[2], 3);
    // peak marker (hidden if the peak lies beyond the chosen range, e.g. 0.2 s view with a long window)
    const peakVisible = h[H_OFF] / 1000 <= t1 + 0.001;
    const px = X(Math.min(h[H_OFF] / 1000, t1)), py = Y(h[H_PEAK] / 100);
    const label = (h[H_PEAK] / 100).toFixed(2) + ' g · ' + (h[H_REP] ? 'Rep' : 'No Rep');
    const bw = W * 0.24, bh = H * 0.05, left = px + bw + 24 > L + pw;
    const bx = left ? px - bw - 14 : px + 14;
    const by = Math.max(T + 2, Math.min(base - bh - 2, py - bh / 2));   // label box stays inside the graph
    if (peakVisible) s += '<circle cx="' + px.toFixed(1) + '" cy="' + py.toFixed(1) + '" r="7" fill="' + COL[active] + '" stroke="#fff" stroke-width="2.5"/>';
    if (peakVisible) s += '<rect x="' + bx.toFixed(1) + '" y="' + by.toFixed(1) + '" width="' + bw.toFixed(1) + '" height="' + bh.toFixed(1) +
         '" rx="8" fill="#1c1c1b" stroke="#444441"/>';
    if (peakVisible) s += '<text x="' + (bx + bw / 2).toFixed(1) + '" y="' + (by + bh / 2 + 6).toFixed(1) + '" fill="#fff" font-size="' + (H * 0.026).toFixed(1) +
         '" font-weight="600" text-anchor="middle">' + label + '</text>';
  }
  svg.innerHTML = s;
}

function plotFrame() {
  if (!plotOpen) return;
  const d = yTarget - yCur;                // ease the scale change over ~0.5 s
  if (Math.abs(d) > 0.05) { yCur += d * 0.2; dirty = true; }
  else if (yCur !== yTarget) { yCur = yTarget; dirty = true; }
  if (dirty) { drawPlot(); dirty = false; }
  requestAnimationFrame(plotFrame);
}

function openSettings() {
  if (st) { pHit = st.hit; pNoise = st.noise; pLock = st.lock; pWin = st.win || 100; }
  hits = []; ovf = [0, 0]; snapData = null; shownId = 0; pinned = false; newestSeen = 0;
  pickScale(); yCur = yTarget; renderSeg();
  renderSetDlg(); renderHits(); renderBuf(); renderHeader();
  $('setPage').classList.add('show');
  plotOpen = true;
  plotPoll();
  requestAnimationFrame(plotFrame);
}
function closeSettings() {
  plotOpen = false;
  sessionPin = '';
  clearTimeout(plotTimer);
  $('setPage').classList.remove('show');
}
window.addEventListener('resize', () => { dirty = true; });

// Steppers. Noise stays below the hit threshold, the window ends before the lockout.
const noiseStep = d => () => { pNoise = Math.min(pHit - 5, Math.max(5, pNoise + d)); renderSetDlg(); };
const hitStep   = d => () => { pHit = Math.min(1600, Math.max(30, pHit + d)); if (pNoise >= pHit) pNoise = pHit - 5; renderSetDlg(); };
const lockStep  = d => () => { pLock = Math.min(3000, Math.max(300, pLock + d)); if (pWin > pLock - 50) pWin = pLock - 50; renderSetDlg(); };
const winStep   = d => () => { pWin = Math.min(300, pLock - 50, Math.max(20, pWin + d)); renderSetDlg(); };
holdRepeat($('nMinus'), noiseStep(-5));
holdRepeat($('nPlus'),  noiseStep(5));
holdRepeat($('hMinus'), hitStep(-10));
holdRepeat($('hPlus'),  hitStep(10));
holdRepeat($('lMinus'), lockStep(-100));
holdRepeat($('lPlus'),  lockStep(100));
holdRepeat($('wMinus'), winStep(-10));
holdRepeat($('wPlus'),  winStep(10));
$('gear').onclick = () => openPin('enter');
$('sCancel').onclick = closeSettings;
$('spClose').onclick = closeSettings;
$('sOk').onclick = () => { const pin = sessionPin; closeSettings(); cmd('/settings?hit=' + pHit + '&noise=' + pNoise + '&lock=' + pLock + '&win=' + pWin + '&pin=' + pin); };

// ---------- PIN pad: protects the settings page ----------
// The Arduino checks the code (/pin?check=), and /settings and /pin only work with it.
// The code is asked every time the gear is tapped; the tablet stays on all day.
let sessionPin = '', pinBuf = '', pinMode = 'enter', newPin = '', pinBusy = false;
function renderPin(msg, cls) {
  document.querySelectorAll('#pinDots i').forEach((d, i) => d.classList.toggle('on', i < pinBuf.length));
  $('pinTitle').textContent = pinMode === 'enter' ? 'Code eingeben' : pinMode === 'new' ? 'Neuer Code' : 'Neuen Code wiederholen';
  if (msg !== undefined) { $('pinMsg').textContent = msg; $('pinMsg').className = 'pmsg' + (cls ? ' ' + cls : ''); }
}
function openPin(mode) {
  pinMode = mode; pinBuf = ''; newPin = '';
  renderPin(mode === 'enter' ? 'Einstellungen sind geschützt' : '4 Ziffern', '');
  $('dlgPin').classList.add('show');
}
function closePin() { $('dlgPin').classList.remove('show'); }
function pinError(msg) {
  const d = $('pinDots'); d.classList.remove('err'); void d.offsetWidth; d.classList.add('err');
  renderPin(msg, 'err');
  setTimeout(() => { d.classList.remove('err'); pinBuf = ''; renderPin(); }, 700);
}
async function pinComplete() {
  pinBusy = true;
  try {
    if (pinMode === 'enter') {
      const r = await get('/pin?check=' + pinBuf);
      if (r.ok) { sessionPin = pinBuf; closePin(); openSettings(); }
      else if (r.wait) pinError('Zu viele Versuche – ' + r.wait + ' s warten');
      else pinError('Falscher Code');
    } else if (pinMode === 'new') {
      newPin = pinBuf; pinMode = 'repeat'; pinBuf = ''; renderPin('Zur Bestätigung', '');
    } else {
      if (pinBuf !== newPin) { pinMode = 'new'; newPin = ''; pinError('Codes stimmen nicht überein'); }
      else {
        const r = await get('/pin?old=' + sessionPin + '&new=' + newPin);
        if (r.ok) { sessionPin = newPin; renderPin('Code geändert', 'okc'); setTimeout(closePin, 900); }
        else pinError('Fehler beim Speichern');
      }
    }
  } catch (e) { pinError('Keine Verbindung'); }
  pinBusy = false;
}
document.querySelectorAll('#pinKeys [data-k]').forEach(b => b.onclick = () => {
  if (pinBusy || pinBuf.length >= 4 || $('pinDots').classList.contains('err')) return;
  pinBuf += b.dataset.k; renderPin();
  if (pinBuf.length === 4) setTimeout(pinComplete, 150);
});
$('pinDel').onclick = () => { if (!pinBusy) { pinBuf = pinBuf.slice(0, -1); renderPin(); } };
$('pinCancel').onclick = closePin;
$('spPin').onclick = () => openPin('new');

// "Kalibrieren": both sensors take their current position as resting position (keep still ~1 s)
let calBusy = false;
$('spCal').onclick = async () => {
  if (calBusy) return;
  calBusy = true;
  const b = $('spCal'), t = $('spCalTxt');
  b.className = 'run'; t.textContent = 'Stillhalten …';
  let ok = false;
  try { ok = (await get('/calib?pin=' + sessionPin)).ok; } catch (e) {}
  b.className = ok ? 'done' : ''; t.textContent = ok ? 'Kalibriert' : 'Fehler';
  setTimeout(() => { b.className = ''; t.textContent = 'Kalibrieren'; calBusy = false; }, 1500);
};

// Reset dialog (tap the big number)
$('reps').onclick = () => $('dlgReset').classList.add('show');
$('rCancel').onclick = () => $('dlgReset').classList.remove('show');
$('rOk').onclick = () => { $('dlgReset').classList.remove('show'); $('go').classList.remove('show'); cmd('/reset'); };

// Tap GO! to close it early
$('go').onclick = () => $('go').classList.remove('show');

poll();
</script>
</body></html>
)rawliteral";
