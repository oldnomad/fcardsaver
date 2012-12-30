<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:func="http://exslt.org/functions"
    xmlns:str="http://exslt.org/strings"
    xmlns:local="local"
    extension-element-prefixes="local func">

<xsl:output method="text" encoding="utf-8"/>

<xsl:param name="header_inc"  select="document('kanjidic2.header.inc')"/>

<xsl:param name="separator"   select="'&#x9;'"/>
<xsl:param name="newline"     select="'&#xA;'"/>
<xsl:param name="quote"       select="'&quot;'"/>
<xsl:param name="escquote"    select="'&quot;&quot;'"/>
<xsl:param name="meaning_sep" select="', '"/>
<xsl:param name="reading_prf" select="'&lt;nobr&gt;'"/>
<xsl:param name="reading_sep" select="',&lt;br/&gt;'"/>
<xsl:param name="reading_suf" select="'&lt;/nobr&gt;'"/>

<func:function name="local:escape-quoted">
    <xsl:param name="node"/>
    <func:result select="str:replace(normalize-space($node), $quote, $escquote)"/>
</func:function>

<xsl:template match="/">
    <xsl:value-of select="$header_inc"/>
    <xsl:value-of select="$newline"/>
    <xsl:apply-templates select="//character"/>
</xsl:template>

<xsl:template match="character">
    <xsl:apply-templates select="literal"/>
    <xsl:value-of select="$separator"/>
    <xsl:apply-templates select="reading_meaning" mode="meaning"/>
    <xsl:value-of select="$separator"/>
    <xsl:apply-templates select="reading_meaning" mode="reading">
        <xsl:with-param name="yomi">ja_kun</xsl:with-param>
    </xsl:apply-templates>
    <xsl:value-of select="$separator"/>
    <xsl:apply-templates select="reading_meaning" mode="reading">
        <xsl:with-param name="yomi">ja_on</xsl:with-param>
    </xsl:apply-templates>
    <xsl:value-of select="$separator"/>
    <xsl:apply-templates select="reading_meaning" mode="nanori"/>
    <xsl:value-of select="$separator"/>
    <xsl:apply-templates select="misc"/>
    <xsl:value-of select="$newline"/>
</xsl:template>

<xsl:template match="misc">
    <xsl:value-of select="$quote"/>
    <xsl:text>Kanji-</xsl:text>
    <xsl:choose>
        <xsl:when test="grade=8">
            <xsl:text>jouyou</xsl:text>
        </xsl:when>
        <xsl:when test="grade=9">
            <xsl:text>jinmei</xsl:text>
        </xsl:when>
        <xsl:when test="grade=10">
            <xsl:text>itai</xsl:text>
        </xsl:when>
        <xsl:when test="grade">
            <xsl:text>G</xsl:text>
            <xsl:value-of select="grade"/>
        </xsl:when>
        <xsl:otherwise>
            <xsl:text>hyougai</xsl:text>
        </xsl:otherwise>
    </xsl:choose>
    <xsl:value-of select="$quote"/>
</xsl:template>

<xsl:template match="reading_meaning" mode="meaning">
    <xsl:value-of select="$quote"/>
    <xsl:for-each select="rmgroup">
        <xsl:if test="position() != 1">
            <xsl:value-of select="$meaning_sep"/>
        </xsl:if>
        <xsl:for-each select="meaning[not(@m_lang)]">
            <xsl:if test="position() != 1">
                <xsl:value-of select="$meaning_sep"/>
            </xsl:if>
            <xsl:value-of select="local:escape-quoted(.)"/>
        </xsl:for-each>
    </xsl:for-each>
    <xsl:value-of select="$quote"/>
</xsl:template>

<xsl:template match="reading_meaning" mode="reading">
    <xsl:param name="yomi"/>
    <xsl:value-of select="$quote"/>
    <xsl:value-of select="$reading_prf"/>
    <xsl:for-each select="rmgroup">
        <xsl:if test="position() != 1">
            <xsl:value-of select="$reading_sep"/>
        </xsl:if>
        <xsl:for-each select="reading[@r_type=$yomi]">
            <xsl:if test="position() != 1">
                <xsl:value-of select="$reading_sep"/>
            </xsl:if>
            <xsl:value-of select="local:escape-quoted(.)"/>
        </xsl:for-each>
    </xsl:for-each>
    <xsl:value-of select="$reading_suf"/>
    <xsl:value-of select="$quote"/>
</xsl:template>

<xsl:template match="reading_meaning" mode="nanori">
    <xsl:value-of select="$quote"/>
    <xsl:value-of select="$reading_prf"/>
    <xsl:for-each select="nanori">
        <xsl:if test="position() != 1">
            <xsl:value-of select="$reading_sep"/>
        </xsl:if>
        <xsl:value-of select="local:escape-quoted(.)"/>
    </xsl:for-each>
    <xsl:value-of select="$reading_suf"/>
    <xsl:value-of select="$quote"/>
</xsl:template>

<xsl:template match="*">
    <xsl:value-of select="$quote"/>
    <xsl:value-of select="local:escape-quoted(.)"/>
    <xsl:value-of select="$quote"/>
</xsl:template>

</xsl:stylesheet>
