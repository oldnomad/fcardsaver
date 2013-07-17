<?xml version="1.0" encoding="utf-8" ?>
<xsl:stylesheet version="1.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:func="http://exslt.org/functions"
    xmlns:str="http://exslt.org/strings"
    xmlns:local="local"
    extension-element-prefixes="local func">

<xsl:output method="text" encoding="utf-8"/>

<xsl:param name="separator"   select="'&#x9;'"/>
<xsl:param name="newline"     select="'&#xA;'"/>
<xsl:param name="quote"       select="'&quot;'"/>
<xsl:param name="escquote"    select="'&quot;&quot;'"/>

<func:function name="local:escape-quoted">
    <xsl:param name="node"/>
    <func:result select="str:replace(normalize-space($node), $quote, $escquote)"/>
</func:function>

<func:function name="local:kana-name">
    <xsl:param name="kana"/>
    <xsl:variable name="name"/>
    <func:result>
        <xsl:choose>
            <xsl:when test="$kana = &quot;hiragana&quot;">Hiragana</xsl:when>
            <xsl:when test="$kana = &quot;katakana&quot;">Katakana</xsl:when>
        </xsl:choose>
    </func:result>
</func:function>

<xsl:template match="/">
    <xsl:apply-templates select="//ch"/>
</xsl:template>

<xsl:template match="ch">
    <xsl:for-each select="r">
        <xsl:if test="not(@katakana)">
            <xsl:call-template name="combine-char">
                <xsl:with-param name="kana">hiragana</xsl:with-param>
            </xsl:call-template>
        </xsl:if>
        <xsl:call-template name="combine-char">
            <xsl:with-param name="kana">katakana</xsl:with-param>
        </xsl:call-template>
    </xsl:for-each>
</xsl:template>

<xsl:template name="combine-char">
    <xsl:param name="kana">hiragana</xsl:param>
    <xsl:variable name="base" select="../@*[name() = $kana]"/>
    <xsl:variable name="mod"><xsl:call-template name="find-modifier">
        <xsl:with-param name="mod" select="@mark"/>
        <xsl:with-param name="kana" select="$kana"/>
    </xsl:call-template></xsl:variable>
    <xsl:variable name="youon" select="@youon"/>
    <xsl:variable name="ch" select="concat($base, $mod)"/>
    <xsl:variable name="set"><xsl:choose>
        <xsl:when test="@set"><xsl:value-of select="@set"/></xsl:when>
        <xsl:when test="string-length($mod) or string-length($youon)">ext</xsl:when>
    </xsl:choose></xsl:variable>
    <xsl:variable name="text" select="."/>
    <xsl:choose>
        <xsl:when test="$youon != &quot;&quot;">
            <xsl:for-each select="//mod[starts-with(@mark, $youon)]">
                <xsl:call-template name="dump-line">
                    <xsl:with-param name="ch" select="concat($ch, @*[name() = $kana])"/>
                    <xsl:with-param name="text" select="concat($text, substring(@mark, 2))"/>
                    <xsl:with-param name="kana" select="$kana"/>
                    <xsl:with-param name="set" select="$set"/>
                </xsl:call-template>
            </xsl:for-each>
        </xsl:when>
        <xsl:otherwise>
            <xsl:call-template name="dump-line">
                <xsl:with-param name="ch" select="$ch"/>
                <xsl:with-param name="text" select="$text"/>
                <xsl:with-param name="kana" select="$kana"/>
                <xsl:with-param name="set" select="$set"/>
            </xsl:call-template>
        </xsl:otherwise>
    </xsl:choose>
</xsl:template>

<xsl:template name="dump-line">
    <xsl:param name="ch"></xsl:param>
    <xsl:param name="text"></xsl:param>
    <xsl:param name="kana"></xsl:param>
    <xsl:param name="set"></xsl:param>
    <xsl:value-of select="$ch"/>
    <xsl:value-of select="$separator"/>
    <xsl:value-of select="$quote"/>
    <xsl:value-of select="local:escape-quoted($text)"/>
    <xsl:value-of select="$quote"/>
    <xsl:value-of select="$separator"/>
    <xsl:value-of select="$quote"/>
    <xsl:value-of select="local:escape-quoted(local:kana-name($kana))"/>
    <xsl:value-of select="$quote"/>
    <xsl:value-of select="$separator"/>
    <xsl:value-of select="$quote"/>
    <xsl:value-of select="local:escape-quoted($set)"/>
    <xsl:value-of select="$quote"/>
    <xsl:value-of select="$newline"/>
</xsl:template>

<xsl:template name="find-modifier">
    <xsl:param name="kana">hiragana</xsl:param>
    <xsl:param name="mod"></xsl:param>
    <xsl:value-of select="//mod[@mark=$mod]/@*[name() = $kana]"/>
</xsl:template>

<xsl:template match="*">
    <xsl:value-of select="$quote"/>
    <xsl:value-of select="local:escape-quoted(.)"/>
    <xsl:value-of select="$quote"/>
</xsl:template>

</xsl:stylesheet>